// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class HOMINGMISSILE_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	
	AProjectileBase();

protected:
	virtual void BeginPlay() override;



public:
	
	virtual void Tick(float DeltaTime) override;
};
