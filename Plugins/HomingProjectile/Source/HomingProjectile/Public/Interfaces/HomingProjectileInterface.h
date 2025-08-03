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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	void SetProjectileTarget(AActor* InTarget);
	virtual void SetProjectileTarget_Implementation(AActor* InTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Homing Projectile")
	AActor* GetProjectileTarget();
	virtual AActor* GetProjectileTarget_Implementation();
};
