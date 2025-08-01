// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HomingProjectileInterface.h"
#include "HomingMissileCharacter.generated.h"

class AProjectileActorBase;

UCLASS(Blueprintable)
class AHomingMissileCharacter : public ACharacter, public IHomingProjectileInterface
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
	
	/** Start HomingProjectileInterface Methods **/
	virtual bool CanBeTargeted_Implementation() override;
	virtual FVector GetFiringSpawnLocation_Implementation() override;
	virtual void FireProjectile_Implementation(AActor* InTargetActor) override;
	/** End of HomingProjectileInterface **/

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileToSpawn;

	UPROPERTY(EditAnywhere)
	UArrowComponent* ProjectileSpawnLocation;

	// Abilities
public:
	void IncreaseBeesCount();
	void CollectPollen();

	int32 GetCurrentBonusDamage() const { return BonusDamage; }
protected:
	int32 AvailableWarriorBeesToSpawn = 2;
	int32 AvailableWorkerBeesToSpawn = 2;
	int32 PollenCollected = 100;
	float CollectionSpeedMultiplier = 1.0f;
	int32 BonusDamage = 0;
};

