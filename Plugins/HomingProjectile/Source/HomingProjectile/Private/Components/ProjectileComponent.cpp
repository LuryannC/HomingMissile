#include "Components/ProjectileComponent.h"

#include "HomingProjectileLog.h"
#include "GameFramework/ProjectileMovementComponent.h"

UProjectileComponent::UProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMovement = GetOwner()->FindComponentByClass<UProjectileMovementComponent>();
	if (!ProjectileMovement)
	{
		UE_LOG(HomingProjectileLog, Error, TEXT("UHomingMissileTargetingComponent::BeginPlay - Couldn't find projectile component."));
		return;
	}

	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = 10000.f;	

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, [this]()
		{
			if (bShouldUpdateHomingDirection)
			{
				UpdateHomingDirection(GetWorld()->DeltaTimeSeconds);
			}
		}, UpdateRate, true);
	}
}

FVector UProjectileComponent::GetPredictedTargetLocation(const float DeltaTime) const
{
	if (!TargetActor)
	{
		//UE_LOG(HomingProjectileLog, Error, TEXT("UProjectileComponent::GetPredictedTargetLocation - Invalid Target."));
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
	
	if (!ProjectileMovement)
	{
		UE_LOG(HomingProjectileLog, Display, TEXT("UHomingMissileTargetingComponent::UpdateHomingDirection - Invalid Projectile Movement Component"));
		return;
	}
	
	if (!TargetActor && bCanSearchForAnotherTarget)
	{
		TargetActor = FindAnotherTarget();
		if (!TargetActor)
		{
			ProjectileMovement->bIsHomingProjectile = false;
			ProjectileMovement->HomingTargetComponent = nullptr;

			// Could also call destruction on this actor as it is no longer useful.
			return;
		}
	}

	if (ProjectileMovement && ProjectileMovement->HomingTargetComponent.IsValid() && ProjectileMovement->HomingTargetComponent != TargetActor->GetRootComponent())
	{
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = TargetActor->GetRootComponent();
	}
	
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
	const FVector PredictedLocation = GetPredictedTargetLocation(DeltaTime);


	const float RandomX = FMath::RandRange(-1,1);
	const float RandomY = FMath::RandRange(-1,1);
	const FVector RandomDirection = FVector(RandomX, RandomY, 0.0f);
	
	const FVector DesiredDirection = PredictedLocation == FVector::ZeroVector ? RandomDirection : (PredictedLocation - CurrentLocation).GetSafeNormal();
	const FVector CurrentDirection = ProjectileMovement->Velocity.GetSafeNormal();

	const FVector NewDirection = FMath::VInterpTo(CurrentDirection, DesiredDirection, DeltaTime, TurnSpeed);

	ProjectileMovement->Velocity = NewDirection * ProjectileMovement->InitialSpeed;


	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), CurrentLocation, PredictedLocation, FColor::Red, false, -1.f, 0, 2.f);
		DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * TurningUnits, FColor::Blue, false, 0.05f, 0, 2.f);
		DrawDebugSphere(GetWorld(), PredictedLocation, PredictionSphereRadius, 12, FColor::Green, false, -1.f);
	}
}

AActor* UProjectileComponent::FindAnotherTarget() const
{
	const FVector CurrentLocation = GetOwner()->GetActorLocation();
		
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	bool bFoundTarget = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		CurrentLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldDynamic),
		FCollisionShape::MakeSphere(SearchRadius),
		Params);

	if (bDrawDebug)
	{
		DrawDebugSphere(GetWorld(), CurrentLocation, SearchRadius, 12, FColor::Purple, false, -1.f);
	}
	
	if (!bFoundTarget)
	{
		return nullptr;
	}
	
	AActor* BestTarget = nullptr;
	float ClosestDistSq = MAX_FLT;

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* Candidate = Result.GetActor();
		if (!Candidate || Candidate == TargetActor) continue;

		if (Candidate->GetClass()->ImplementsInterface(UHomingProjectileInterface::StaticClass()))
		{
			const bool bCanBeTargeted = IHomingProjectileInterface::Execute_CanBeTargeted(Candidate);
			const uint8 TargetTeam = IHomingProjectileInterface::Execute_GetProjectileTeam(Candidate);
			const uint8 OwnerTeam = IHomingProjectileInterface::Execute_GetProjectileTeam(GetOwner());
			
			if (bCanBeTargeted && OwnerTeam != TargetTeam)
			{
				float DistSq = FVector::DistSquared(CurrentLocation, Candidate->GetActorLocation());
				if (DistSq < ClosestDistSq)
				{
					ClosestDistSq = DistSq;
					BestTarget = Candidate;
				}
			}
		}
	}
	
	if (bDrawDebug && BestTarget)
	{
		DrawDebugLine(GetWorld(), CurrentLocation, BestTarget->GetActorLocation(), FColor::Cyan, false, 2.f, 0, 2.f);
	}
	
	return BestTarget;
}
