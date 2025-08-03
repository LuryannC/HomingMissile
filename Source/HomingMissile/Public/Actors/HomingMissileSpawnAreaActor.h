// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingMissileSpawnAreaActor.generated.h"

enum EEntityTeam : uint8;
class UBoxComponent;

UCLASS()
class HOMINGMISSILE_API AHomingMissileSpawnAreaActor : public AActor
{
	GENERATED_BODY()

public:
	AHomingMissileSpawnAreaActor();

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEntityTeam> EntityToSpawn;
};
