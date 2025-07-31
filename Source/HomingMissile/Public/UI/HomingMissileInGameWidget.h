// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomingMissileInGameWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileInGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TimerText;
	
	TObjectPtr<UButton> ShopButton;

private:
	void InitializeGameWidget();

	UFUNCTION()
	void OnShopButtonClicked();

	UFUNCTION()
	void UpdateTimerText(const int32& Seconds);
};
