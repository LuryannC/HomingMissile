// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/HomingMissileUpgradeButton.h"

#include "HomingMissileCharacter.h"

void UHomingMissileUpgradeButton::OnBuyUpgrade()
{
	CurrentIndex++;

	CurrentIndex = FMath::Clamp(CurrentIndex, 0, PriceUpgradeArray.Num());
	
	SetIsEnabled(CanBuyUpgrade());

	// There isn't more upgrades available 
	if (CurrentIndex == PriceUpgradeArray.Num())
	{
		SetIsEnabled(false);
	}
}

int32 UHomingMissileUpgradeButton::GetUpgradePrice()
{
	if (PriceUpgradeArray.IsValidIndex(CurrentIndex))
	{
		return PriceUpgradeArray[CurrentIndex].Price;
	}
	return 0;
}

bool UHomingMissileUpgradeButton::CanBuyUpgrade()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			if (PriceUpgradeArray.IsValidIndex(CurrentIndex))
			{
				return Player->GetPollenCollected() >= PriceUpgradeArray[CurrentIndex].Price;
			}
		}
	}

	return false;
}
