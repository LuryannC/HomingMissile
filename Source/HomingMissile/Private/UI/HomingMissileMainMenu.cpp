// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HomingMissileMainMenu.h"

#include "HomingMissileGameMode.h"
#include "HomingMissilePlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/HomingMissileRootWidget.h"

void UHomingMissileMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UHomingMissileMainMenu::OnStartGameButtonClicked);
	}
	
	if (TutorialButton)
	{
		TutorialButton->OnClicked.AddDynamic(this, &UHomingMissileMainMenu::OnTutorialButtonClicked);
	}
	
	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UHomingMissileMainMenu::OnExitButtonClicked);
	}
}

void UHomingMissileMainMenu::OnStartGameButtonClicked()
{
	if (GetWorld())
	{
		if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->StartGame();
		}
	}
	
	if (InGameWidgetClass && GetWorld() && GetOwningPlayer())
	{
		if (AHomingMissilePlayerController* PC = Cast<AHomingMissilePlayerController>(GetOwningPlayer()))
		{
			PC->GetRootWidget()->PushWidget(InGameWidgetClass);

			const FInputModeGameAndUI InputModeGameAndUI;
			PC->SetInputMode(InputModeGameAndUI);
		}
	}
}

void UHomingMissileMainMenu::OnTutorialButtonClicked()
{
	if (TutorialWidgetClass && GetWorld() && GetOwningPlayer())
	{
		if (AHomingMissilePlayerController* PC = Cast<AHomingMissilePlayerController>(GetOwningPlayer()))
		{
			PC->GetRootWidget()->PushWidget(TutorialWidgetClass);
		}
	}
}

void UHomingMissileMainMenu::OnExitButtonClicked()
{
	if (GetOwningPlayer())
	{
		UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
	}	
}
