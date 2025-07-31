// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/HomingMissileTargetingComponent.h"

#include "GameFramework/ProjectileMovementComponent.h"


UHomingMissileTargetingComponent::UHomingMissileTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHomingMissileTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovement)
	{
		UE_LOG(LogTemp, Error, TEXT("UHomingMissileTargetingComponent::BeginPlay - Couldn't find projectile component."));
	}
}

void UHomingMissileTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateHomingDirection(DeltaTime);
}

void UHomingMissileTargetingComponent::SetTarget(AActor* Target)
{
	TargetActor = Target;
	if (ProjectileMovement && TargetActor)
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingAccelerationMagnitude = 10000.f;
		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	}
}

FVector UHomingMissileTargetingComponent::GetPredictedTargetLocation()
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Error, TEXT("UHomingMissileTargetingComponent::GetPredictedTargetLocation - Invalid Target."));
		return FVector::ZeroVector;
	}

	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector TargetVelocity = TargetActor->GetVelocity();

	return TargetLocation + TargetVelocity * PredictionTime;
}

void UHomingMissileTargetingComponent::UpdateHomingDirection(float DeltaTime)
{
	if (ActorTeam == Pollen)
	{
		return;
	}
	
	if (!TargetActor || !ProjectileMovement)
	{
		UE_LOG(LogTemp, Display, TEXT("UHomingMissileTargetingComponent::UpdateHomingDirection - Invalid Target or Projectile Movement Component"));
		return;
	}

	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const FVector PredictedLocation = GetPredictedTargetLocation();
	const FVector DesiredDirection = (PredictedLocation - CurrentLocation).GetSafeNormal();
	
	ProjectileMovement->Velocity = DesiredDirection * ProjectileMovement->InitialSpeed;
	
	// Debug
	DrawDebugLine(GetWorld(), CurrentLocation, PredictedLocation, FColor::Red, false, -1.f, 0, 2.f);
	DrawDebugSphere(GetWorld(), PredictedLocation, 250.f, 12, FColor::Green, false, -1.f);
}
