// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingMissileEntityBase.h"
#include "HomingMissileBee.generated.h"

UCLASS()
class HOMINGMISSILE_API AHomingMissileBee : public AHomingMissileEntityBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHomingMissileBee();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
