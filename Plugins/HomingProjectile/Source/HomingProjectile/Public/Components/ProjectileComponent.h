// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "ProjectileComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndPurpose);

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

	UPROPERTY(BlueprintAssignable)
	FOnEndPurpose OnEndPurpose;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/* How many steps ahead should it predict */
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float PredictionTime = 0.5f;

	/* Decide if the target should be a Homing Projectile */
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	bool bShouldUpdateHomingDirection = true;

	/* In case the Projectile can look for another target but fails, should it end its purpose? If Not it will just roam around. */
	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection", EditConditionHides))
	bool bShouldCallEndGoalOnFailingToFindTarget = false;

	/* When the current target become null, should it look for another one? */
	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection", EditConditionHides))
	bool bCanSearchForAnotherTarget = true;

	/* Radius to search for another target. */
	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection && bCanSearchForAnotherTarget", EditConditionHides))
	float SearchRadius = 1000.0f;

	/* Update frequency of the UpdateHomingDirection function - Won't change at runtime. */
	UPROPERTY(EditAnywhere, Category="Homing Projectile", meta=(EditCondition = "bShouldUpdateHomingDirection", EditConditionHides))
	float UpdateRate = 0.01f;

	/* Speed that the projectile turns to face the target */
	UPROPERTY(EditAnywhere, Category="Homing Projectile")
	float TurnSpeed = 5.0f;

	/* Show debug draw (Sphere, Lines) */
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
