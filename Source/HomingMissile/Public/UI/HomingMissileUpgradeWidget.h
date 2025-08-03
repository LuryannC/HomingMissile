// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomingMissileUpgradeWidget.generated.h"

class UTextBlock;
class UHomingMissileUpgradeButton;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHomingMissileUpgradeButton* UpgradeCapacityButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHomingMissileUpgradeButton* UpgradeBonusDamageButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHomingMissileUpgradeButton* UpgradeAttackCooldownReductionButton;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHomingMissileUpgradeButton* UpgradeNumberOfWorkersButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UHomingMissileUpgradeButton* UpgradeNumberOfWarriorsButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* PollenCountText;

	UFUNCTION(BlueprintCallable)
	void UpdateWidget();

private:
	UFUNCTION()
	void OnUpgradeCapacity();

	UFUNCTION()
	void OnUpgradeBonusDamage();
	
	UFUNCTION()
	void OnUpgradeAttackCooldownReduction();
	
	UFUNCTION()
	void OnUpgradeNumberOfWorkers();
	
	UFUNCTION()
	void OnUpgradeNumberOfWarriors();


	void SetupButtons();
	void UpdatePollenCountText() const;
	
	TArray<UHomingMissileUpgradeButton*> UpgradeButtons;
};
