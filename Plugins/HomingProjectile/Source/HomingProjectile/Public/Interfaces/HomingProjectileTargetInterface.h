#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HomingProjectileTargetInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHomingProjectileTargetInterface: public UInterface
{
	GENERATED_BODY()
};
 
class HOMINGPROJECTILE_API IHomingProjectileTargetInterface
{
	GENERATED_BODY()
 
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	bool CanBeTargeted();
	virtual bool CanBeTargeted_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	bool CanTarget(AActor* InTarget);
	virtual bool CanTarget_Implementation(AActor* InTarget);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	FVector GetFiringSpawnLocation();
	virtual FVector GetFiringSpawnLocation_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	void FireProjectile(AActor* InTargetActor);
	virtual void FireProjectile_Implementation(AActor* InTargetActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	uint8 GetProjectileTeam();
	virtual uint8 GetProjectileTeam_Implementation();
};
