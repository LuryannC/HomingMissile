// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileActorBase.h"
#include "Components/ProjectileComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileActorBase::AProjectileActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileComponent>(TEXT("ProjectileComponent"));
	if (ProjectileComponent)
	{
		ProjectileComponent->OnEndPurpose.AddDynamic(this, &AProjectileActorBase::AProjectileActorBase::BP_OnEndPurpose);
	}
}

void AProjectileActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileActorBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ProjectileComponent)
	{
		ProjectileComponent->OnEndPurpose.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AProjectileActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileActorBase::SetProjectileTarget_Implementation(AActor* InTarget)
{
	if (InTarget && ProjectileTarget != InTarget)
	{
		ProjectileTarget = InTarget;
	}
}

AActor* AProjectileActorBase::GetProjectileTarget_Implementation()
{
	return ProjectileTarget;
}

void AProjectileActorBase::BP_OnEndPurpose_Implementation()
{
	ProjectileMovementComponent->bIsHomingProjectile = false;
	ProjectileMovementComponent->HomingTargetComponent = nullptr;
}