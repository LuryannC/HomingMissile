// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissileGameState.h"

#include "HomingMissileGameMode.h"

void AHomingMissileGameState::StartTimer()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(
			RoundTimerHandle,
			this,
			&AHomingMissileGameState::UpdateElapsedTime,
			1.0f,
			true);
	}
}

void AHomingMissileGameState::UpdateElapsedTime()
{
	ElapsedSeconds++;
	OnTimeUpdatedEvent.Broadcast(ElapsedSeconds);


	if (bEnableRoundDebug && ElapsedSeconds >= MaxSecondsToSwitchRound)
	{
		if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(AuthorityGameMode))
		{
			GM->DebugEndRound();
		}
	}
}
