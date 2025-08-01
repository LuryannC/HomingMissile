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

	AActor* FindAnotherTarget() const;

	FTimerHandle UpdateTimerHandle;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float PredictionTime = 0.5f;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	bool bShouldUpdateHomingDirection = true;

	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection", EditConditionHides))
	bool bCanSearchForAnotherTarget = true;

	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection && bCanSearchForAnotherTarget", EditConditionHides))
	float SearchRadius = 1000.0f;

	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection", EditConditionHides))
	float UpdateRate = 0.01f;
	
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float TurnSpeed = 5.0f;

	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	bool bDrawDebug = false;

	/* Shows which direction the projectile is facing, this is to increase the size of the debug line. */
	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bDrawDebug", EditConditionHides))
	float TurningUnits = 200.0f;

	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bDrawDebug", EditConditionHides))
	float PredictionSphereRadius = 250.0f;

private:
	FVector GetPredictedTargetLocation(const float DeltaTime) const;
	void UpdateHomingDirection(float DeltaTime);
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	AActor* TargetActor;
};
