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
		PC->InitializePlayer();
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
	OnRoundStartEvent.Broadcast();
}

void AHomingMissileGameMode::EndRound()
{
	if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GameState))
	{
		GS->RoundTimerHandle.Invalidate();
		
		FRoundStatistics RoundStatistics{};
		RoundStatistics.ElapsedSeconds = GS->ElapsedSeconds;
		RoundStatistics.LastCompletedRound = GS->CurrentRound;
		RoundStatistics.WaspsKilled = GS->WaspsKilled;
		RoundStatistics.PollenCollected = GS->PollenCollected;

		OnRoundEndEvent.Broadcast(RoundStatistics);

	
		if (GS->CurrentRound > RoundParamsCurveTable->GetRowMap().Num())
		{
			EndGame();
			return;
		}

		GS->CurrentRound++;
	}
}

void AHomingMissileGameMode::EndGame()
{
	if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GameState))
	{
		GS->Reset();
	}
}

// DEBUG
void AHomingMissileGameMode::DebugEndRound()
{
	EndRound();
}
