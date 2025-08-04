// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HomingProjectileWarriors.h"
#include "HomingMissileCharacter.h"
#include "HomingMissileGameMode.h"
#include "Components/ProjectileComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHomingProjectileWarriors::AHomingProjectileWarriors()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHomingProjectileWarriors::BeginPlay()
{
	Super::BeginPlay();
	
}


void AHomingProjectileWarriors::ApplyDamage(AActor* TargetActor, int32 Amount)
{
	if (!TargetActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("AHomingProjectileBase::ApplyDamage - Invalid Target"))
		return;
	}

	if (!CanAttack())
	{
		return;
	}
	
	if (AHomingProjectileBase* Target = Cast<AHomingProjectileBase>(TargetActor))
	{
		if (Target->ProjectileTeam != ProjectileTeam)
		{
			int32 TotalDamage = Amount;
			
			// Player bonus damage
			if (GetOwner())
			{
				if (AHomingMissileCharacter* Character = Cast<AHomingMissileCharacter>(GetOwner()))
				{
					TotalDamage += Character->GetBonusDamage();
				}
			}
			
			Target->TakeDamage(TotalDamage);
			BP_OnApplyDamage();
			LastTimeUsedAttack = GetWorld()->GetTimeSeconds();
			
			UE_LOG(LogTemp, Display, TEXT("AHomingProjectileBase::ApplyDamage - %s Applied %i to %s"), *GetName(), Amount, *TargetActor->GetName());

			if ((Target->GetHealth() - Amount) <= 0)
			{
				SetProjectileTarget_Implementation(ProjectileComponent->FindAnotherTarget());
			}
		}
	}
}

void AHomingProjectileWarriors::BP_OnApplyDamage_Implementation()
{
}

void AHomingProjectileWarriors::OnDeath()
{
	if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnEntityDestroyedEvent.Broadcast(this);
	}
	Super::OnDeath();
}


// Called every frame
void AHomingProjectileWarriors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AHomingProjectileWarriors::CanAttack() const
{
	float TotalCooldown = AttackCooldown;
	
	// Player Cooldown reduction
	if (GetOwner())
	{
		if (AHomingMissileCharacter* Character = Cast<AHomingMissileCharacter>(GetOwner()))
		{
			TotalCooldown = AttackCooldown * (1.f - (Character->GetCooldownReductionPercentage() / 100.f));
		}
	}
	
	const bool bCanAttack = GetWorld()->GetTimeSeconds() - LastTimeUsedAttack >= TotalCooldown; 
	return bCanAttack;
}

void AHomingProjectileWarriors::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		PerformAction(OtherActor, ActionBaseValue);
	}
}

void AHomingProjectileWarriors::PerformAction(AActor* TargetActor, int32 Amount)
{
	ApplyDamage(TargetActor, Amount);
}

void AHomingProjectileWarriors::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnEntityDestroyedEvent.Broadcast(this);
	}
	Super::EndPlay(EndPlayReason);
}
