// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "HomingMissileDataTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HomingMissileGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEndDelegate, FRoundStatistics, RoundStatistics);


UCLASS(minimalapi)
class AHomingMissileGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHomingMissileGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void StartGame();
	
	UPROPERTY(BlueprintAssignable)
	FOnRoundStartDelegate OnRoundStartEvent;

	UPROPERTY(BlueprintAssignable)
	FOnRoundEndDelegate OnRoundEndEvent;

	// Debug
	void DebugEndRound();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	UCurveTable* RoundParamsCurveTable;
	
private:
	void StartRound();
	void EndRound();
	void EndGame();
};



