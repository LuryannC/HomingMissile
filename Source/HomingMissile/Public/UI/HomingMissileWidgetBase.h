// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomingMissileWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	void ReturnToPreviousWidget();
};
