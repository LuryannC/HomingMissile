// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "HomingMissileMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class HOMINGMISSILE_API UHomingMissileMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> TutorialButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TutorialWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InGameWidgetClass;

private:

	UFUNCTION()
	void OnStartGameButtonClicked();

	UFUNCTION()
	void OnTutorialButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
	
};
