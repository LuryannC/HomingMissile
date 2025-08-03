// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingProjectileBase.h"
#include "HomingProjectileWorkers.generated.h"

UCLASS()
class HOMINGMISSILE_API AHomingProjectileWorkers : public AHomingProjectileBase
{
	GENERATED_BODY()

public:
	AHomingProjectileWorkers();

	virtual void PerformAction(AActor* TargetActor, int32 Amount) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EEntityTeam> TargetTeam;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
