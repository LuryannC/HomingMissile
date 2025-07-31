// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingMissileDataTypes.h"
#include "Components/ActorComponent.h"
#include "HomingMissileTargetingComponent.generated.h"


class UProjectileMovementComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HOMINGMISSILE_API UHomingMissileTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHomingMissileTargetingComponent();

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* Target);

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EEntityTeam> ActorTeam = Bee;
	
	UPROPERTY(EditAnywhere)
	float PredictionTime = 0.5f;

	UPROPERTY(EditAnywhere)
	float HomingStrength = 500.0f;

private:
	FVector GetPredictedTargetLocation();
	void UpdateHomingDirection(float DeltaTime);
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;
};
