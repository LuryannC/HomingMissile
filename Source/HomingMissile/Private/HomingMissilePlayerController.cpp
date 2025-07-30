// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissile/Public/HomingMissilePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "HomingMissile/Public/HomingMissileCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
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
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AHomingMissilePlayerController::OnInputStarted);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AHomingMissilePlayerController::OnSetDestinationTriggered);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AHomingMissilePlayerController::OnSetDestinationReleased);
		// EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AHomingMissilePlayerController::OnSetDestinationReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}
//
// void AHomingMissilePlayerController::OnInputStarted()
// {
// 	StopMovement();
// }
//
// // Triggered every frame when the input is held down
// void AHomingMissilePlayerController::OnSetDestinationTriggered()
// {
// 	// We flag that the input is being pressed
// 	FollowTime += GetWorld()->GetDeltaSeconds();
// 	
// 	// We look for the location in the world where the player has pressed the input
// 	FHitResult Hit;
// 	bool bHitSuccessful = false;
// 	if (bIsTouch)
// 	{
// 		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
// 	}
// 	else
// 	{
// 		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
// 	}
//
// 	// If we hit a surface, cache the location
// 	if (bHitSuccessful)
// 	{
// 		CachedDestination = Hit.Location;
// 	}
// 	
// 	// Move towards mouse pointer or touch
// 	APawn* ControlledPawn = GetPawn();
// 	if (ControlledPawn != nullptr)
// 	{
// 		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
// 		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
// 	}
// }
//
// void AHomingMissilePlayerController::OnSetDestinationReleased()
// {
// 	// If it was a short press
// 	if (FollowTime <= ShortPressThreshold)
// 	{
// 		// We move there and spawn some particles
// 		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
// 		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
// 	}
//
// 	FollowTime = 0.f;
// }