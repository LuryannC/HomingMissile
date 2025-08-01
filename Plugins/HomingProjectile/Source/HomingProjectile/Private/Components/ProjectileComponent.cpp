#include "Components/ProjectileComponent.h"

#include "HomingProjectileLog.h"
#include "GameFramework/ProjectileMovementComponent.h"

UProjectileComponent::UProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovement)
	{
		UE_LOG(HomingProjectileLog, Error, TEXT("UHomingMissileTargetingComponent::BeginPlay - Couldn't find projectile component."));
	}
	else
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingAccelerationMagnitude = 10000.f;		
	}
}

void UProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bShouldUpdateHomingDirection)
	{
		UpdateHomingDirection(DeltaTime);
	}
}

FVector UProjectileComponent::GetPredictedTargetLocation(const float DeltaTime) const
{
	if (!TargetActor)
	{
		UE_LOG(HomingProjectileLog, Error, TEXT("UProjectileComponent::GetPredictedTargetLocation - Invalid Target."));
		return FVector::ZeroVector;
	}

	const FVector TargetLocation = TargetActor->GetActorLocation();
	const FVector TargetVelocity = TargetActor->GetVelocity();

	return TargetLocation + TargetVelocity * (DeltaTime * PredictionTime);
}

void UProjectileComponent::UpdateHomingDirection(const float DeltaTime)
{
	if (!TargetActor)
	{
		if (const IHomingProjectileInterface* ProjectileInterface = Cast<IHomingProjectileInterface>(GetOwner()))
		{
			TargetActor = ProjectileInterface->Execute_GetProjectileTarget(GetOwner());
		}
	}
	
	if (!TargetActor || !ProjectileMovement)
	{
		UE_LOG(HomingProjectileLog, Display, TEXT("UHomingMissileTargetingComponent::UpdateHomingDirection - Invalid Target or Projectile Movement Component"));
		return;
	}

	if (ProjectileMovement && ProjectileMovement->HomingTargetComponent != TargetActor->GetRootComponent())
	{
		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	}
	
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const FVector PredictedLocation = GetPredictedTargetLocation(DeltaTime);
	const FVector DesiredDirection = (PredictedLocation - CurrentLocation).GetSafeNormal();
	const FVector CurrentDirection = ProjectileMovement->Velocity.GetSafeNormal();

	FVector NewDirection = FMath::VInterpTo(CurrentDirection, DesiredDirection, DeltaTime, TurnSpeed);

	ProjectileMovement->Velocity = NewDirection * ProjectileMovement->InitialSpeed;


	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), CurrentLocation, PredictedLocation, FColor::Red, false, -1.f, 0, 2.f);
		DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * TurningUnits, FColor::Blue, false, 0.05f, 0, 2.f);
		DrawDebugSphere(GetWorld(), PredictedLocation, 250.f, 12, FColor::Green, false, -1.f);
	}
}
