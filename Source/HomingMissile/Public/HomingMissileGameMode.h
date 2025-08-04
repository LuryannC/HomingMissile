// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "HomingMissileDataTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HomingMissileGameMode.generated.h"


class AHomingProjectileWarriors;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundEndDelegate, FRoundStatistics, RoundStatistics);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEntityDestroyed, AActor*, Entity);


USTRUCT(BlueprintType)
struct FSpawnedEntity
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Game Params|Classes")
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category="Game Params|Classes")
	TEnumAsByte<EEntityTeam> Team;
};

UCLASS(minimalapi)
class AHomingMissileGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHomingMissileGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION()
	void StartGame();
	
	UPROPERTY(BlueprintAssignable)
	FOnRoundStartDelegate OnRoundStartEvent;

	UPROPERTY(BlueprintAssignable)
	FOnRoundEndDelegate OnRoundEndEvent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	UCurveTable* RoundParamsCurveTable;
	
	UPROPERTY(BlueprintReadWrite, Category="Homing Missile")
	TArray<AActor*> PollenInGame;

	UPROPERTY(BlueprintReadWrite, Category="Homing Missile")
	TArray<AActor*> WaspsInGame;
	
	UPROPERTY(BlueprintReadWrite, Category="Homing Missile")
	TArray<AActor*> BeesInGame;
	
	FOnEntityDestroyed OnEntityDestroyedEvent;

	// Debug
	void DebugEndRound();

	UPROPERTY(EditAnywhere, Category="Debug")
	bool bShowDebugLog = false;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	FName SpawnWaspsCurveTableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	FName PollenToSpawnCurveTableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	float MinDistanceBetweenSpawns = 200.f;

	/* Used to try fixing the spawn location for the entities */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	int32 MaxAttemptsToSpawn = 20;
	
	UPROPERTY(EditAnywhere, Category="Game Params|Spawn")
	FSpawnedEntity WaspEntity;
	
	UPROPERTY(EditAnywhere, Category="Game Params|Spawn")
	FSpawnedEntity PollenEntity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Params")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

private:
	void StartRound();

	UFUNCTION(Exec) // Adding for debug
	void EndRound();

	UFUNCTION(Exec) // Adding for debug
	void EndGame();
	
	void CheckRoundState();
	bool CanEndRound();

	void SpawnEntities();
	TArray<AActor*> SpawnEntity(const FSpawnedEntity SpawnedEntity, const FName TableName) const;
	FVector GetRandomPointOnFloor(EEntityTeam SpawnTeam) const;

	UFUNCTION()
	void OnEntityDestroyed(AActor* Entity);

	int32 GetCurveTableColumnCount() const;
};



