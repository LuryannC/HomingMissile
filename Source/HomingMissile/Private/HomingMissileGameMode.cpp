// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissileGameMode.h"
#include "HomingMissilePlayerController.h"
#include "HomingMissileCharacter.h"
#include "HomingMissileGameState.h"
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
		//TODO: Initialize player menu.
		//PC->InitializePlayer();
	}
}

void AHomingMissileGameMode::StartGame()
{
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

// DEBUG
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
