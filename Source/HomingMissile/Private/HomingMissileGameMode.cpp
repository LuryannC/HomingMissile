// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissileGameMode.h"
#include "HomingMissilePlayerController.h"
#include "HomingMissileGameState.h"
#include "Actors/HomingMissileSpawnAreaActor.h"
#include "Actors/HomingProjectileWarriors.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	SpawnEntities();
	StartRound();
}

void AHomingMissileGameMode::StartRound()
{
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
		
		GS->ResetTimer();
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
	SpawnEntity(WaspEntity, SpawnWaspsCurveTableName);
	SpawnEntity(PollenEntity, PollenToSpawnCurveTableName);	
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

			const float Floor = Box.Min.Z = Box.Max.Z;
			RandomPoint.Z = Floor;

			return RandomPoint;
		}
	}
	
	return FVector::ZeroVector;
}

void AHomingMissileGameMode::SpawnEntity(const FSpawnedEntity SpawnedEntity, const FName TableName) const
{
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
						GetWorld()->SpawnActor<AActor>(SpawnedEntity.ActorClass, CandidateLocation, FRotator::ZeroRotator, SpawnParameters);

						++SpawnedCount;
					}

					--MaxAttempts;
				}
			}
		}
	}
}
