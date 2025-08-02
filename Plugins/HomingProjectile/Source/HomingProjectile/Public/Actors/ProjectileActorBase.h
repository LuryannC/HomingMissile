// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "Interfaces/HomingProjectileTargetInterface.h"
#include "ProjectileActorBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UTargetingComponent;
class UProjectileComponent;

UCLASS(Abstract)
class HOMINGPROJECTILE_API AProjectileActorBase : public AActor, public IHomingProjectileInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileActorBase();
	
	// ~ Start HomingProjectileInterface Methods
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* This is called in case the bShouldCallEndGoalOnFailingToFindTarget is true, to let the developer decide what to do with this projectile. */
	UFUNCTION(BlueprintNativeEvent)
	void BP_OnEndPurpose();
	virtual void BP_OnEndPurpose_Implementation();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
