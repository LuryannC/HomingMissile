// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileWidgetBase.h"
#include "HomingMissilePlayerController.h"
#include "UI/HomingMissileRootWidget.h"

void UHomingMissileWidgetBase::ReturnToPreviousWidget()
{
	if (GetOwningPlayer())
	{
		if (AHomingMissilePlayerController* PC = Cast<AHomingMissilePlayerController>(GetOwningPlayer()))
		{
			PC->GetRootWidget()->ReturnToPreviousWidget();
		}
	}
}
