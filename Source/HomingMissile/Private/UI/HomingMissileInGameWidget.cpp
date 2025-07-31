// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileInGameWidget.h"

#include "HomingMissileGameMode.h"
#include "HomingMissileGameState.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UHomingMissileInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ShopButton)
	{
		ShopButton.Get()->OnClicked.AddDynamic(this, &UHomingMissileInGameWidget::OnShopButtonClicked);
	}

	InitializeGameWidget();
}

void UHomingMissileInGameWidget::InitializeGameWidget()
{
	if (GetWorld())
	{
		if (AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(GetWorld()->GetGameState()))
		{
			GS->OnTimeUpdatedEvent.AddDynamic(this, &UHomingMissileInGameWidget::UpdateTimerText);
		}
	}
}

void UHomingMissileInGameWidget::OnShopButtonClicked()
{
	UE_LOG(LogTemp, Display, TEXT("UHomingMissileInGameWidget::OnShopButtonClicked - Shop clicked!"))
}

void UHomingMissileInGameWidget::UpdateTimerText(const int32& Seconds)
{
	int32 TotalMinutes = Seconds / 60;
	int32 TotalSeconds = Seconds % 60;

	TimerText.Get()->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), TotalMinutes, TotalSeconds)));
}
