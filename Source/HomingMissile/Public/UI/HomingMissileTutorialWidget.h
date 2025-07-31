// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HomingMissileWidgetBase.h"
#include "Components/Button.h"
#include "HomingMissileTutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileTutorialWidget : public UHomingMissileWidgetBase
{
	GENERATED_BODY()

public:

protected:
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ReturnButton;

private:

	UFUNCTION()
	void OnReturnClicked();
};
