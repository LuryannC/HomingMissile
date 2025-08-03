#include "Actors/HomingProjectileBase.h"
#include "HomingMissileCharacter.h"
#include "Components/ProjectileComponent.h"
#include "Components/SphereComponent.h"
#include "HomingMissileCharacter.h"


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

void AHomingProjectileBase::PerformAction(AActor* TargetActor, int32 Amount)
{
}

void AHomingProjectileBase::BP_PerformAction_Implementation()
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