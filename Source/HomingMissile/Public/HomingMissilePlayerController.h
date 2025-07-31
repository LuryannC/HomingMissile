// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "HomingMissilePlayerController.generated.h"

/** Forward declaration to improve compiling times */
class AHomingMissileEntityBase;
class UHomingMissileRootWidget;
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;
	
protected:

	virtual void SetupInputComponent() override;

	void OnMouseClicked();

	void FireProjectile(FHitResult HitResult);
	
	virtual void BeginPlay();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHomingMissileRootWidget> RootWidgetClass;

	UPROPERTY()
	TObjectPtr<UHomingMissileRootWidget> RootWidget;

	UPROPERTY(EditAnywhere, Category="Bees")
	AActor* ProjectileObjectSpawnPoint;
};


