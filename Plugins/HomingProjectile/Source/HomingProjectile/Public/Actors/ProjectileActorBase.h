// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "ProjectileActorBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UTargetingComponent;
class UProjectileComponent;

UCLASS()
class HOMINGPROJECTILE_API AProjectileActorBase : public AActor, public IHomingProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileActorBase();
	
	// ~ Start HomingProjectileInterface Methods
	virtual bool CanBeTargeted_Implementation() override;
	virtual void SetProjectileTarget_Implementation(AActor* InTarget) override;
	virtual AActor* GetProjectileTarget_Implementation() override;
	// ~ End of HomingProjectileInterface
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileComponent* ProjectileComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY()
	AActor* ProjectileTarget;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Homing Projectile")
	bool bCanBeTargeted = false;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
