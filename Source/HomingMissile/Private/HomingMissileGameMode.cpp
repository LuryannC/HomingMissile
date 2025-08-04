// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissileGameMode.h"

#include "HomingMissileCharacter.h"
#include "HomingMissilePlayerController.h"
#include "HomingMissileGameState.h"
#include "Actors/HomingMissileSpawnAreaActor.h"
#include "Actors/HomingProjectileWarriors.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/HomingMissileRootWidget.h"
#include "UObject/ConstructorHelpers.h"

AHomingMissileGameMode::AHomingMissileGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHomingMissilePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AHomingMissileGameMode::BeginPlay()
{
	Super::BeginPlay();
	OnEntityDestroyedEvent.AddDynamic(this, &AHomingMissileGameMode::OnEntityDestroyed);
}

void AHomingMissileGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnEntityDestroyedEvent.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void AHomingMissileGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AHomingMissilePlayerController* PC = Cast<AHomingMissilePlayerController>(NewPlayer))
	{
		// Initialize player menu
		PC->InitializePlayer();
	}
}

void AHomingMissileGameMode::StartGame()
{
	StartRound();
	if (GetWorld())
	{
		FTimerHandle RoundEndTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(RoundEndTimerHandle,
											   [this]()
											   {
												   CheckRoundState();
											   }, 1.0f, true);
	}
}

void AHomingMissileGameMode::StartRound()
{
	PollenInGame.Empty();
	WaspsInGame.Empty();
	BeesInGame.Empty();

	SpawnEntities();
	
	if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GameState))
	{
		GS->StartTimer();
	}

	if (bShowDebugLog)
	{
		UE_LOG(LogTemp, Display, TEXT("AHomingMissileGameMode::StartRound - Round Started"))
	}
	
	OnRoundStartEvent.Broadcast();
}

void AHomingMissileGameMode::EndRound()
{
	if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GameState))
	{
		FRoundStatistics RoundStatistics;
		RoundStatistics.ElapsedSeconds = GS->ElapsedSeconds;
		RoundStatistics.LastCompletedRound = GS->CurrentRound;
		RoundStatistics.WaspsKilled = GS->WaspsKilled;
		RoundStatistics.PollenCollected = GS->PollenCollected;

		OnRoundEndEvent.Broadcast(RoundStatistics);


		if (GS->CurrentRound > GetCurveTableColumnCount())
		{
			EndGame();
			return;
		}
		
		if (bShowDebugLog)
		{
			UE_LOG(LogTemp, Display, TEXT("AHomingMissileGameMode::EndRound - End Round"))
		}


		GS->ElapsedSeconds = 0;
		GS->CurrentRound++;

		if (AHomingMissileCharacter* Character = Cast<AHomingMissileCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
		{
			Character->ResetSpawnValues();
		}
		
		GS->ResetTimer();
		StartRound();
	}
}

void AHomingMissileGameMode::EndGame()
{
	if (bShowDebugLog)
	{
		UE_LOG(LogTemp, Display, TEXT("AHomingMissileGameMode::EndRound - End Game"))
	}
	
	if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GameState))
	{
		GS->ResetGameValues();
	}

	UGameplayStatics::SetGamePaused(this, true);
	if (AHomingMissilePlayerController* PC = Cast<AHomingMissilePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->GetRootWidget()->PushWidget(GameOverWidgetClass);
	}
}

void AHomingMissileGameMode::CheckRoundState()
{
	if (CanEndRound())
	{
		EndRound();
	}
}

bool AHomingMissileGameMode::CanEndRound()
{
	// TODO: Event dispatcher to remove the dead ones from the array.
	if (GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		const AHomingMissileCharacter* Character = Cast<AHomingMissileCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (WaspsInGame.IsEmpty())
		{
			if (!PollenInGame.IsEmpty())
			{
				// Remove the nullptr to avoid bugs
				PollenInGame.RemoveAll([](const AActor* Pollen)
				{
					return Pollen == nullptr;
				});
				
				return Character->GetTotalWorkerBeesToSpawn() < 0;
			}
			return true;
		}
		else
		{
			if (!BeesInGame.IsEmpty())
			{
				// Remove the nullptr to avoid bugs
				BeesInGame.RemoveAll([](const AActor* Bee)
				{
					return Bee == nullptr;
				});
			}
			
			if (BeesInGame.IsEmpty() && Character->GetTotalWarriorBeesToSpawn() <= 0)
			{
				EndRound();
			}	
		}
	}
	return false;
}

void AHomingMissileGameMode::DebugEndRound()
{
	EndRound();
}

int32 AHomingMissileGameMode::GetCurveTableColumnCount() const
{
	if (!RoundParamsCurveTable || RoundParamsCurveTable->GetRowMap().IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("AHomingMissileGameMode::GetCurveTableColumnCount - Invalid or empty Curve Table"))
		return 0;
	}

	for (const TPair<FName, FRealCurve*>& RowPair : RoundParamsCurveTable->GetRowMap())
	{
		const int32 Size = RowPair.Value->GetNumKeys() - 1;
		return Size;
	}
	
	return 0;
}

void AHomingMissileGameMode::SpawnEntities()
{
	WaspsInGame = SpawnEntity(WaspEntity, SpawnWaspsCurveTableName);
	PollenInGame = SpawnEntity(PollenEntity, PollenToSpawnCurveTableName);	
}

FVector AHomingMissileGameMode::GetRandomPointOnFloor(EEntityTeam SpawnTeam) const
{
	TArray<AActor*> OutActors;	
	UGameplayStatics::GetAllActorsOfClass(this, AHomingMissileSpawnAreaActor::StaticClass(), OutActors);

	AActor* SpawnAreaActor = nullptr;

	for (const auto& Actor : OutActors)
	{
		if (AHomingMissileSpawnAreaActor* CurrenActorArea = Cast<AHomingMissileSpawnAreaActor>(Actor))
		{
			if (CurrenActorArea->EntityToSpawn == SpawnTeam)
			{
				SpawnAreaActor = CurrenActorArea;
				break;
			} 
		}
	}

	if (SpawnAreaActor)
	{
		if (UBoxComponent* SpawnArea =  SpawnAreaActor->FindComponentByClass<UBoxComponent>())
		{
			FBox Box{SpawnArea->GetComponentLocation(), SpawnArea->GetScaledBoxExtent()};
			FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox_Box(Box);

			// const float Floor = Box.Min.Z - Box.Max.Z;
			const float Floor = 50.0f; // I will hardcode this for now just to avoid bugs on spawn
			RandomPoint.Z = Floor;

			return RandomPoint;
		}
	}
	
	return FVector::ZeroVector;
}

void AHomingMissileGameMode::OnEntityDestroyed(AActor* Entity)
{
	if (WaspsInGame.Contains(Entity))
	{
		WaspsInGame.Remove(Entity);
	}
	else if (PollenInGame.Contains(Entity))
	{
		PollenInGame.Remove(Entity);
	}
}

TArray<AActor*> AHomingMissileGameMode::SpawnEntity(const FSpawnedEntity SpawnedEntity, const FName TableName) const
{
	TArray<AActor*> OutActors;
	
	if (const FRealCurve* Curve = RoundParamsCurveTable->FindCurve(TableName, FString::Printf(TEXT("%s"), *TableName.ToString())))
	{
		if (const AHomingMissileGameState* GS = GetGameState<AHomingMissileGameState>())
		{
			const float AmountToSpawnFloat = Curve->Eval(GS->CurrentRound);
			const int32 AmountToSpawn =  FMath::CeilToInt(AmountToSpawnFloat);

			if (GetWorld())
			{

				int32 SpawnedCount = 0;
				int32 MaxAttempts = MaxAttemptsToSpawn;
				TArray<FVector> SpawnedLocations;
				
				while (SpawnedCount < AmountToSpawn && MaxAttempts > 0)
				{
					FVector CandidateLocation = GetRandomPointOnFloor(SpawnedEntity.Team);
					bool bTooClose = false;

					for (const FVector& OtherLocation : SpawnedLocations)
					{
						if (FVector::DistSquared(CandidateLocation, OtherLocation) < FMath::Square(MinDistanceBetweenSpawns))
						{
							bTooClose = true;
							break;
						}
					}

					if (!bTooClose)
					{
						SpawnedLocations.Add(CandidateLocation);

						FActorSpawnParameters SpawnParameters;
						SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
						AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnedEntity.ActorClass, CandidateLocation, FRotator::ZeroRotator, SpawnParameters);
						OutActors.Add(SpawnedActor);

						++SpawnedCount;
					}

					--MaxAttempts;
				}
			}
		}
	}

	return OutActors;
}
