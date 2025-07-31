// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileTutorialWidget.h"

void UHomingMissileTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UHomingMissileTutorialWidget::OnReturnClicked);
	}
}

void UHomingMissileTutorialWidget::OnReturnClicked()
{
	ReturnToPreviousWidget();
}
