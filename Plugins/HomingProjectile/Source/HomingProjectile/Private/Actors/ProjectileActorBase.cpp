// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileActorBase.h"
#include "Components/ProjectileComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileActorBase::AProjectileActorBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComponent = CreateDefaultSubobject<UProjectileComponent>(TEXT("ProjectileComponent"));
}

void AProjectileActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AProjectileActorBase::CanBeTargeted_Implementation()
{
	return false;
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
