// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "Interfaces/HomingProjectileTargetInterface.h"
#include "HomingMissileCharacter.generated.h"

class AProjectileActorBase;

UCLASS(Blueprintable)
class AHomingMissileCharacter : public ACharacter, public IHomingProjectileInterface, public IHomingProjectileTargetInterface
{
	GENERATED_BODY()

public:
	AHomingMissileCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Start IHomingProjectileTargetInterface Methods **/
	virtual bool CanBeTargeted_Implementation() override;
	virtual FVector GetFiringSpawnLocation_Implementation() override;
	virtual void FireProjectile_Implementation(AActor* InTargetActor) override;
	/** End of IHomingProjectileTargetInterface **/

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	UPROPERTY(EditAnywhere, Category="Homing Missile|Classes")
	TSubclassOf<AActor> BeeWarriorProjectileClass;

	UPROPERTY(EditAnywhere, Category="Homing Missile|Classes")
	TSubclassOf<AActor> BeeWorkerProjectileClass;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ProjectileSpawnLocation;

	
public:
	
	int32 GetAvailableWarriorBeesToSpawn() const { return AvailableWarriorBeesToSpawn; }
	int32 GetAvailableWorkerBeesToSpawn() const { return AvailableWorkerBeesToSpawn; }
	int32 GetBonusDamage() const { return BonusDamage; }
	float GetCooldownReductionPercentage() const { return CooldownReductionPercentage; }
	int32 GetPollenCollected() const { return  PollenCollected; }
	int32 GetPollenCollectionCapacity() const { return PollenCollectionCapacity; }

	void SetAvailableWarriorBeesToSpawn(const int32 Amount) { AvailableWarriorBeesToSpawn = Amount; }
	void SetAvailableWorkerBeesToSpawn(const int32 Amount) { AvailableWorkerBeesToSpawn = Amount; }
	void SetBonusDamage(const int32 Amount) {BonusDamage = Amount; }
	void SetCooldownReductionPercentage(const float Amount)  { CooldownReductionPercentage = Amount; }
	void CollectPollen(const int32 AmountToCollect) { PollenCollected += AmountToCollect; }

protected:
	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	int32 AvailableWarriorBeesToSpawn = 2;

	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	int32 AvailableWorkerBeesToSpawn = 2;

	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	int32 PollenCollected = 100;
	
	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	int32 PollenCollectionCapacity = 30;

	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	int32 BonusDamage = 0;
	
	UPROPERTY(EditAnywhere, Category="Homing Missile|Augments")
	float CooldownReductionPercentage = 0.0f;
};

