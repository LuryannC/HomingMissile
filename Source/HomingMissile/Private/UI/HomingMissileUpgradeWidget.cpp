// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileUpgradeWidget.h"

#include "HomingMissileCharacter.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/Components/HomingMissileUpgradeButton.h"

void UHomingMissileUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	if (UpgradeCapacityButton)
	{
		UpgradeCapacityButton->OnClicked.AddDynamic(this, &UHomingMissileUpgradeWidget::OnUpgradeCapacity);
		UpgradeButtons.Add(UpgradeCapacityButton);
	}
	if (UpgradeBonusDamageButton)
	{
		UpgradeBonusDamageButton->OnClicked.AddDynamic(this, &UHomingMissileUpgradeWidget::OnUpgradeBonusDamage);
		UpgradeButtons.Add(UpgradeBonusDamageButton);
	}
	if (UpgradeAttackCooldownReductionButton)
	{
		UpgradeAttackCooldownReductionButton->OnClicked.AddDynamic(this, &UHomingMissileUpgradeWidget::OnUpgradeAttackCooldownReduction);
		UpgradeButtons.Add(UpgradeAttackCooldownReductionButton);
	}
	if (UpgradeNumberOfWorkersButton)
	{
		UpgradeNumberOfWorkersButton->OnClicked.AddDynamic(this, &UHomingMissileUpgradeWidget::OnUpgradeNumberOfWorkers);
		UpgradeButtons.Add(UpgradeNumberOfWorkersButton);
	}
	if (UpgradeNumberOfWarriorsButton)
	{
		UpgradeNumberOfWarriorsButton->OnClicked.AddDynamic(this, &UHomingMissileUpgradeWidget::OnUpgradeNumberOfWarriors);
		UpgradeButtons.Add(UpgradeNumberOfWarriorsButton);
	}

	if (PollenCountText)
	{
		UpdatePollenCountText();
	}

	SetupButtons();
}

void UHomingMissileUpgradeWidget::NativeDestruct()
{
	Super::NativeDestruct();

	UpgradeCapacityButton->OnClicked.RemoveAll(this);
	UpgradeBonusDamageButton->OnClicked.RemoveAll(this);
	UpgradeAttackCooldownReductionButton->OnClicked.RemoveAll(this);
	UpgradeNumberOfWorkersButton->OnClicked.RemoveAll(this);
	UpgradeNumberOfWarriorsButton->OnClicked.RemoveAll(this);
}

void UHomingMissileUpgradeWidget::SetupButtons()
{
	for (const auto& Button : UpgradeButtons)
	{
		Button->SetIsEnabled(Button->CanBuyUpgrade());
	}

	UpdatePollenCountText();
}

void UHomingMissileUpgradeWidget::UpdateWidget()
{
	UpdatePollenCountText();
	SetupButtons();
}

void UHomingMissileUpgradeWidget::OnUpgradeCapacity()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			Player->SetPollenCollectionCapacityUpgrade(UpgradeCapacityButton->PriceUpgradeArray[UpgradeCapacityButton->CurrentIndex].AmountToUpgrade);
			UpgradeCapacityButton->OnBuyUpgrade();

			// Reset buttons state.
			SetupButtons();
		}
	}
}

void UHomingMissileUpgradeWidget::OnUpgradeBonusDamage()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			Player->SetBonusDamage(UpgradeBonusDamageButton->PriceUpgradeArray[UpgradeBonusDamageButton->CurrentIndex].AmountToUpgrade);
			UpgradeBonusDamageButton->OnBuyUpgrade();

			// Reset buttons state.
			SetupButtons();
		}
	}
}

void UHomingMissileUpgradeWidget::OnUpgradeAttackCooldownReduction()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			Player->SetCooldownReductionPercentage(UpgradeAttackCooldownReductionButton->PriceUpgradeArray[UpgradeAttackCooldownReductionButton->CurrentIndex].AmountToUpgrade);
			UpgradeAttackCooldownReductionButton->OnBuyUpgrade();

			// Reset buttons state.
			SetupButtons();
		}
	}
}

void UHomingMissileUpgradeWidget::OnUpgradeNumberOfWorkers()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			Player->SetAvailableWorkerBeesToSpawnUpgrade(UpgradeNumberOfWorkersButton->PriceUpgradeArray[UpgradeNumberOfWorkersButton->CurrentIndex].AmountToUpgrade);
			UpgradeNumberOfWorkersButton->OnBuyUpgrade();

			// Reset buttons state.
			SetupButtons();
		}
	}
}

void UHomingMissileUpgradeWidget::OnUpgradeNumberOfWarriors()
{
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			Player->SetAvailableWarriorBeesToSpawnUpgrade(UpgradeNumberOfWarriorsButton->PriceUpgradeArray[UpgradeNumberOfWarriorsButton->CurrentIndex].AmountToUpgrade);
			UpgradeNumberOfWarriorsButton->OnBuyUpgrade();

			// Reset buttons state.
			SetupButtons();
		}
	}
}

void UHomingMissileUpgradeWidget::UpdatePollenCountText() const
{
	if (!PollenCountText)
	{
		return;
	}
	
	if (GetOwningPlayer() && GetOwningPlayer()->GetPawn())
	{
		if (const AHomingMissileCharacter* Player = Cast<AHomingMissileCharacter>(GetOwningPlayer()->GetPawn()))
		{
			PollenCountText->SetText(FText::FromString(FString::FromInt(Player->GetPollenCollected())));
		}
	}
}
