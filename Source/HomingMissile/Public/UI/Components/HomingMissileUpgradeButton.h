// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "HomingMissileUpgradeButton.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FPriceUpgradeStructure
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmountToUpgrade;
};
/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileUpgradeButton : public UButton
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentIndex = 0;

	UPROPERTY(EditAnywhere)
	TArray<FPriceUpgradeStructure> PriceUpgradeArray;

	void OnBuyUpgrade();

	UFUNCTION(BlueprintCallable)
	int32 GetUpgradePrice();
	
	bool CanBuyUpgrade();
};
