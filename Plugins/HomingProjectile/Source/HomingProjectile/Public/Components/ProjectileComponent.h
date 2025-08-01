// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "ProjectileComponent.generated.h"


class UProjectileMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOMINGPROJECTILE_API UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProjectileComponent();

	void SetTargetActor(AActor* InActor) { TargetActor = InActor; }

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float PredictionTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	bool bShouldUpdateHomingDirection = true;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float TurnSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bDrawDebug"))
	float TurningUnits = 200.0f;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector GetPredictedTargetLocation(const float DeltaTime) const;
	void UpdateHomingDirection(float DeltaTime);
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	AActor* TargetActor;
};
