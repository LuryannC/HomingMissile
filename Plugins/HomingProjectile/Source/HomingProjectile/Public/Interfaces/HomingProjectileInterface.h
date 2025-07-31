#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HomingProjectileInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHomingProjectileInterface: public UInterface
{
	GENERATED_BODY()
};
 
class HOMINGPROJECTILE_API IHomingProjectileInterface
{
	GENERATED_BODY()
 
public:
	/** for the projectile **/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	bool CanBeTargeted();
	virtual bool CanBeTargeted_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	void SetProjectileTarget(AActor* InTarget);
	virtual void SetProjectileTarget_Implementation(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	AActor* GetProjectileTarget();
	virtual AActor* GetProjectileTarget_Implementation();

	/** For the Shooter **/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	FVector GetFiringSpawnLocation();
	virtual FVector GetFiringSpawnLocation_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	void FireProjectile(AActor* InTargetActor);
	virtual void FireProjectile_Implementation(AActor* InTargetActor);	
};
