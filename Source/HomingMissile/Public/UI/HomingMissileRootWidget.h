// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Overlay.h"
#include "HomingMissileRootWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileRootWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void InitializeRootWidget();
	
	UFUNCTION(BlueprintCallable)
	void PushWidget(UClass* WidgetClassToPush);

	UFUNCTION(BlueprintCallable)
	void ReturnToPreviousWidget();

protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> WidgetAreaOverlay;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> DefaultStartupWidget;

private:

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentChildWidget;

	UPROPERTY()
	TSubclassOf<UUserWidget> PreviousChildWidgetClass;
};
