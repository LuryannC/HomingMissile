#include "Actors/HomingProjectileBase.h"

#include "HomingMissileCharacter.h"
#include "Components/ProjectileComponent.h"
#include "Components/SphereComponent.h"


AHomingProjectileBase::AHomingProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(MeshComponent);
	MeshComponent->SetMobility(EComponentMobility::Movable);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetMobility(EComponentMobility::Movable);
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHomingProjectileBase::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AHomingProjectileBase::OnSphereEndOverlap);
}

bool AHomingProjectileBase::CanBeTargeted_Implementation()
{
	return bCanBeTargeted;
}

void AHomingProjectileBase::SetProjectileTarget_Implementation(AActor* InTarget)
{
	ProjectileComponent->SetTargetActor(InTarget);
}

uint8 AHomingProjectileBase::GetProjectileTeam_Implementation()
{
	return ProjectileTeam;
}

void AHomingProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHomingProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHomingProjectileBase::TakeDamage(const int32 Amount)
{
	const int32 FinalDamage = FMath::Clamp(Amount, 0, Amount);
	HealthBase -= FinalDamage;

	if (HealthBase <= 0)
	{
		OnDeath();
	}

	BP_OnTakeDamage(Amount);
}

void AHomingProjectileBase::BP_OnTakeDamage_Implementation(int32 Amount)
{
}

void AHomingProjectileBase::ApplyDamage(AActor* TargetActor, int32 Amount)
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

void AHomingProjectileBase::BP_OnApplyDamage_Implementation()
{
}

void AHomingProjectileBase::OnDeath()
{
	if (ProjectileComponent)
	{
		GetWorldTimerManager().ClearTimer(ProjectileComponent->UpdateTimerHandle);
	}
	
	BP_OnDeath();
}

void AHomingProjectileBase::BP_OnDeath_Implementation()
{
	Destroy();
}


void AHomingProjectileBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AHomingProjectileBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

bool AHomingProjectileBase::CanAttack() const
{
	const bool bCanAttack = GetWorld()->GetTimeSeconds() - LastTimeUsedAttack >= AttackCooldown; 
	return bCanAttack;
}
