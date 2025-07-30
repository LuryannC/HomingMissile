// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileRootWidget.h"

void UHomingMissileRootWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		InitializeRootWidget();
	});	
}

void UHomingMissileRootWidget::InitializeRootWidget()
{
	if (DefaultStartupWidget && GetWorld())
	{
		if (UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), DefaultStartupWidget))
		{
			PushWidget(WidgetInstance);
		}
	}
}

void UHomingMissileRootWidget::PushWidget(UUserWidget* WidgetToPush)
{
	if (!WidgetToPush)
	{
		UE_LOG(LogTemp, Error, TEXT("UHomingMissileRootWidget::PushWidget - Tried to push invalid widget."))
		return;
	}
	
	PreviousChildWidget = CurrentChildWidget;
	if (PreviousChildWidget)
	{
		WidgetAreaOverlay.Get()->RemoveChild(PreviousChildWidget);
	}

	CurrentChildWidget = WidgetToPush;
	WidgetAreaOverlay.Get()->AddChild(WidgetToPush);
}
