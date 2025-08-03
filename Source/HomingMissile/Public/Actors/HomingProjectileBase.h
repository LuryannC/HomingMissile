// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ProjectileActorBase.h"
#include "HomingProjectileBase.generated.h"

enum EEntityTeam : uint8;

/*
 * Inherited class from AProjectileActorBase in Homing Projectile Plugin
 */
UCLASS()
class HOMINGMISSILE_API AHomingProjectileBase : public AProjectileActorBase, public IHomingProjectileTargetInterface
{
	GENERATED_BODY()

public:
	AHomingProjectileBase();

	/** Start IHomingProjectileInterface Methods **/
	virtual void SetProjectileTarget_Implementation(AActor* InTarget) override;
	virtual uint8 GetProjectileTeam_Implementation() override;
	/** End of IHomingProjectileInterface **/

	/** Start IHomingProjectileTargetInterface Methods **/
	virtual bool CanBeTargeted_Implementation() override;
	/** End of IHomingProjectileTargetInterface **/

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(const int32 Amount);

	/* Useful for adding VFX / SFX */
	UFUNCTION(BlueprintNativeEvent)
	void BP_OnTakeDamage(int32 Amount);
	void BP_OnTakeDamage_Implementation(int32 Amount);

	UFUNCTION(BlueprintCallable)
	virtual void PerformAction(AActor* TargetActor, int32 Amount);

	/* Useful for adding VFX / SFX */
	UFUNCTION(BlueprintNativeEvent)
	void BP_PerformAction();
	void BP_PerformAction_Implementation();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnDeath();

	UFUNCTION(BlueprintNativeEvent)
	void BP_OnDeath();
	void BP_OnDeath_Implementation();

	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const { return HealthBase; };

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Homing Projectile")
	bool bCanBeTargeted = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing Projectile")
	int32 HealthBase = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing Projectile")
	int32 ActionBaseValue = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing Projectile")
	float AttackCooldown = 3.0f;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Homing Projectile")
	TEnumAsByte<EEntityTeam> ProjectileTeam;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
