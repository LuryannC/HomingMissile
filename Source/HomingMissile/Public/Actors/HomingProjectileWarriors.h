// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingProjectileBase.h"
#include "HomingProjectileWarriors.generated.h"

UCLASS()
class HOMINGMISSILE_API AHomingProjectileWarriors : public AHomingProjectileBase
{
	GENERATED_BODY()

public:

	AHomingProjectileWarriors();

	virtual void PerformAction(AActor* TargetActor, int32 Amount) override;

	UFUNCTION(BlueprintCallable)
	virtual void ApplyDamage(AActor* TargetActor, int32 Amount);

	/* Useful for adding VFX / SFX */
	UFUNCTION(BlueprintNativeEvent)
	void BP_OnApplyDamage();
	void BP_OnApplyDamage_Implementation();


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	
	bool CanAttack() const;
	float LastTimeUsedAttack = -FLT_MAX;
};
