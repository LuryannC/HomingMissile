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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UMeshComponent> ProjectileMesh;
	
	UPROPERTY(BlueprintReadWrite, Category="Projectile Settings")
	float ProjectileSpeed = 100.0f;

	UPROPERTY(BlueprintReadWrite, Category="Projectile Settings")
	float DamageAreaRadius = 250.0f;
	
	UPROPERTY(BlueprintReadWrite, Category="Projectile Settings")
	float ProjectileDamage = 100.0f;

public:
	
	virtual void Tick(float DeltaTime) override;
};
