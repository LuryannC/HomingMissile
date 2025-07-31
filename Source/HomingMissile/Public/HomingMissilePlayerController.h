// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "HomingMissilePlayerController.generated.h"

class AHomingMissileEntityBase;
class UHomingMissileRootWidget;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AHomingMissilePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHomingMissilePlayerController();

	void InitializePlayer();

	UHomingMissileRootWidget* GetRootWidget();
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;
	
protected:

	virtual void SetupInputComponent() override;

	void OnMouseClicked();

	void FireProjectile(FHitResult HitResult);
	
	// To add mapping context
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHomingMissileRootWidget> RootWidgetClass;

	UPROPERTY()
	TObjectPtr<UHomingMissileRootWidget> RootWidget;

	UPROPERTY(EditAnywhere, Category="Bees")
	TSubclassOf<AHomingMissileEntityBase> SpawnObjectClass;

	UPROPERTY(EditAnywhere, Category="Bees")
	AActor* ProjectileObjectSpawnPoint;

private:
};


