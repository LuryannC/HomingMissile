// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HomingProjectileWorkers.h"

#include "HomingMissileCharacter.h"
#include "HomingMissileDataTypes.h"


AHomingProjectileWorkers::AHomingProjectileWorkers()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHomingProjectileWorkers::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHomingProjectileWorkers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHomingProjectileWorkers::PerformAction(AActor* TargetActor, int32 Amount)
{
	if (TargetActor->GetClass()->ImplementsInterface(UHomingProjectileTargetInterface::StaticClass()))
	{
		if (IHomingProjectileTargetInterface::Execute_GetProjectileTeam(TargetActor) == TargetTeam)
		{
			// Collect Pollen
			if (GetOwner())
			{
				if (AHomingMissileCharacter* Character = Cast<AHomingMissileCharacter>(GetOwner()))
				{
					const int32 PollenCollected = FMath::Clamp(Amount, ActionBaseValue, Character->GetPollenCollectionCapacity());
					Character->CollectPollen(PollenCollected);
				}
			}
		}
	}
}
