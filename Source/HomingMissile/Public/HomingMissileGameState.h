// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HomingMissileGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdatedDelegate, const int32&, TotalSeconds);

/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API AHomingMissileGameState : public AGameStateBase
{
	GENERATED_BODY()


public:

	void StartTimer();


	UPROPERTY(BlueprintAssignable)
	FOnTimeUpdatedDelegate OnTimeUpdatedEvent;
	
	int32 CurrentRound = 0;
	int32 ElapsedSeconds = 0;
	int32 WaspsKilled = 0;
	int32 PollenCollected = 0;
	
	FTimerHandle RoundTimerHandle;

	void ResetGameValues();
	void ResetTimer();
	
protected:
	
	// DEBUG AREA
	UPROPERTY(EditAnywhere, Category="Debug")
	bool bEnableRoundDebug = false;

	UPROPERTY(EditAnywhere, Category="Debug")
	int32 MaxSecondsToSwitchRound = 10;

private:
	
	void UpdateElapsedTime();
	
	
};
