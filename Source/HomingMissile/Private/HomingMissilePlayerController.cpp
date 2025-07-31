// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissile/Public/HomingMissilePlayerController.h"
#include "GameFramework/Pawn.h"
#include "HomingMissile/Public/HomingMissileCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "UI/HomingMissileRootWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AHomingMissilePlayerController::AHomingMissilePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AHomingMissilePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AHomingMissilePlayerController::InitializePlayer()
{
	// Instantiate the root widget
	if (UUserWidget* RootWidgetInstance = CreateWidget(this, RootWidgetClass))
	{
		RootWidget = Cast<UHomingMissileRootWidget>(RootWidgetInstance);
		if (RootWidget)
		{
			RootWidget->AddToViewport();
		}
	}
}

UHomingMissileRootWidget* AHomingMissilePlayerController::GetRootWidget()
{
	if (RootWidget)
	{
		return RootWidget;
	}
	
	UE_LOG(LogTemp, Error, TEXT("AHomingMissilePlayerController::GetRootWidget - Invalid Root Widget."));
	return nullptr;
}

void AHomingMissilePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AHomingMissilePlayerController::OnMouseClicked);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AHomingMissilePlayerController::OnMouseClicked()
{
	FHitResult Hit;
	bool bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	if (bHitSuccessful)
	{
		FireProjectile(Hit);
	}
}

void AHomingMissilePlayerController::FireProjectile(FHitResult HitResult)
{
	if (const IHomingProjectileInterface* OtherActorProjectileInterface = Cast<IHomingProjectileInterface>(HitResult.GetActor()))
	{
		if (OtherActorProjectileInterface->Execute_CanBeTargeted(HitResult.GetActor()))
		{
			if (GetPawn())
			{
				if (const IHomingProjectileInterface* ControlledPawnHomingInterface = Cast<IHomingProjectileInterface>(GetPawn()))
				{
					ControlledPawnHomingInterface->Execute_FireProjectile(GetPawn(), HitResult.GetActor());
				}
			}			
		}
	}
}