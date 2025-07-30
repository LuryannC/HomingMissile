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

	if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(AuthorityGameMode))
	{
		if (bEnableRoundDebug && ElapsedSeconds >= MaxSecondsToSwitchRound)
		{
			GM->DebugEndRound();
		}
	}
}
