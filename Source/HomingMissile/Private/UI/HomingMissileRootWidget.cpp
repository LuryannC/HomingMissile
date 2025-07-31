// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileRootWidget.h"

void UHomingMissileRootWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		InitializeRootWidget();
	});
}

void UHomingMissileRootWidget::InitializeRootWidget()
{
	if (DefaultStartupWidget && GetWorld())
	{
		PushWidget(DefaultStartupWidget);
	}
}

void UHomingMissileRootWidget::PushWidget(UClass* WidgetClassToPush)
{
	if (!WidgetClassToPush && !WidgetClassToPush->IsChildOf(UUserWidget::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("UHomingMissileRootWidget::PushWidget - Invalid class type, Please use UUserWidget."))
		return;
	}
	
	if (GetWorld() && GetOwningPlayer())
	{
		if (UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClassToPush))
		{
			if (!WidgetInstance)
			{
				UE_LOG(LogTemp, Error, TEXT("UHomingMissileRootWidget::PushWidget - Tried to push invalid widget."))
				return;
			}

			if (CurrentChildWidget)
			{
				CurrentChildWidget.Get()->RemoveFromParent();
				PreviousChildWidgetClass = CurrentChildWidget.GetClass();
			}

			CurrentChildWidget = WidgetInstance;

			WidgetAreaOverlay.Get()->AddChild(WidgetInstance);
		}
	}
}

void UHomingMissileRootWidget::ReturnToPreviousWidget()
{
	PushWidget(PreviousChildWidgetClass);
}
