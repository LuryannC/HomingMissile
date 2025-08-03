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

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	/** Start IHomingProjectileTargetInterface Methods **/
	virtual bool CanBeTargeted_Implementation() override;
	virtual FVector GetFiringSpawnLocation_Implementation() override;
	virtual void FireProjectile_Implementation(AActor* InTargetActor) override;
	/** End of IHomingProjectileTargetInterface **/

	virtual void PossessedBy(AController* NewController) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Homing Missile|Game Params")
	FName AvailableWarriorBeesToSpawnTableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Homing Missile|Game Params")
	FName AvailableWorkerBeesToSpawnTableName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Homing Missile|Game Params")
	FName PollenCollectionCapacityTableName;
public:
	
	int32 GetAvailableWarriorBeesToSpawn() const { return AvailableWarriorBeesToSpawn; }
	int32 GetAvailableWorkerBeesToSpawn() const { return AvailableWorkerBeesToSpawn; }
	int32 GetBonusDamage() const { return BonusDamage; }
	float GetCooldownReductionPercentage() const { return CooldownReductionPercentage; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetPollenCollected() const { return  PollenCollected; }
	
	int32 GetPollenCollectionCapacity() const { return PollenCollectionCapacity; }

	void SetAvailableWarriorBeesToSpawnUpgrade(const int32 Amount);
	void SetAvailableWorkerBeesToSpawnUpgrade(const int32 Amount);
	void SetPollenCollectionCapacityUpgrade(const int32 Amount) { PollenCollectionCapacityUpgrade = Amount; }
	void SetBonusDamage(const int32 Amount) {BonusDamage = Amount; }
	void SetCooldownReductionPercentage(const float Amount)  { CooldownReductionPercentage = Amount; }
	void CollectPollen(const int32 AmountToCollect) { PollenCollected += AmountToCollect; }
	void BuyUpgrade(const int32 Cost) { PollenCollected -= Cost; }

	int32 GetTotalWarriorBeesToSpawn() const;
	int32 GetTotalWorkerBeesToSpawn() const;

protected:
	int32 AvailableWarriorBeesToSpawn = 0;
	int32 AvailableWarriorBeesToSpawnUpgrade = 0;
	int32 TotalWarriorBees = 0;
	
	int32 AvailableWorkerBeesToSpawn = 0;
	int32 AvailableWorkerBeesToSpawnUpgrade = 0;
	int32 TotalWorkerBees = 0;
	
	int32 PollenCollected = 0;
	int32 PollenCollectionCapacity = 5;
	int32 PollenCollectionCapacityUpgrade = 0;
	int32 BonusDamage = 0;
	float CooldownReductionPercentage = 0.0f;

private:
	void SetupInitialDataValues();
	float SetupDataDefaultValue(const FName TableRowName) const;
};

