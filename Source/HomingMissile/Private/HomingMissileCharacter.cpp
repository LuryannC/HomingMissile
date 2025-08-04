// Copyright Epic Games, Inc. All Rights Reserved.

#include "HomingMissileCharacter.h"

#include "HomingMissileDataTypes.h"
#include "HomingMissileGameMode.h"
#include "HomingMissileGameState.h"
#include "Actors/ProjectileActorBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

AHomingMissileCharacter::AHomingMissileCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	ProjectileSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnLocation"));
}

void AHomingMissileCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ResetSpawnValues();
}

bool AHomingMissileCharacter::CanBeTargeted_Implementation()
{
	return false;
}

FVector AHomingMissileCharacter::GetFiringSpawnLocation_Implementation()
{
	if (ProjectileSpawnLocation)
	{
		return ProjectileSpawnLocation->GetComponentLocation();
	}
	return FVector::ZeroVector;
}

void AHomingMissileCharacter::FireProjectile_Implementation(AActor* InTargetActor)
{
	if (!InTargetActor || !ProjectileSpawnLocation)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	if (!InTargetActor->GetClass()->ImplementsInterface(UHomingProjectileTargetInterface::StaticClass()))
	{
		return;
	}

	const EEntityTeam TargetTeam = static_cast<EEntityTeam>(IHomingProjectileTargetInterface::Execute_GetProjectileTeam(InTargetActor));
	UClass* ClassToSpawn = (TargetTeam == EEntityTeam::Wasp) ? BeeWarriorProjectileClass : BeeWorkerProjectileClass;

	if (!ClassToSpawn)
	{
		return;
	}
	
	bool bShouldSpawn = false;

	switch (TargetTeam)
	{
	case EEntityTeam::Wasp:
		if (GetTotalWarriorBeesToSpawn() > 0)
		{
			TotalWarriorBees--;
			bShouldSpawn = true;
		}
		break;

	case EEntityTeam::Pollen:
		if (GetTotalWorkerBeesToSpawn() > 0)
		{
			TotalWorkerBees--;
			bShouldSpawn = true;
		}
		break;

	default:
		break;
	}

	if (bShouldSpawn)
	{
		AProjectileActorBase* Projectile = GetWorld()->SpawnActor<AProjectileActorBase>(
		ClassToSpawn, 
		ProjectileSpawnLocation->GetComponentTransform(), 
		SpawnParams
		);		

		if (!Projectile) return;

		Projectile->SetProjectileTarget_Implementation(InTargetActor);
	}
}

void AHomingMissileCharacter::PossessedBy(AController* NewController)
{
	SetupInitialDataValues();
}

void AHomingMissileCharacter::SetAvailableWarriorBeesToSpawnUpgrade(const int32 Amount)
{
	AvailableWarriorBeesToSpawnUpgrade += Amount;
	TotalWarriorBees = AvailableWarriorBeesToSpawn + AvailableWarriorBeesToSpawnUpgrade;
}

void AHomingMissileCharacter::SetAvailableWorkerBeesToSpawnUpgrade(const int32 Amount)
{
	AvailableWorkerBeesToSpawnUpgrade += Amount;
	TotalWorkerBees = AvailableWorkerBeesToSpawn + AvailableWorkerBeesToSpawnUpgrade;
}

int32 AHomingMissileCharacter::GetTotalWarriorBeesToSpawn() const
{
	return TotalWarriorBees;
}

int32 AHomingMissileCharacter::GetTotalWorkerBeesToSpawn() const
{
	return TotalWorkerBees;
}

void AHomingMissileCharacter::ResetSpawnValues()
{
	// I know this is really bad!!
	SetAvailableWarriorBeesToSpawnUpgrade(0);
	SetAvailableWorkerBeesToSpawnUpgrade(0);
}

void AHomingMissileCharacter::SetupInitialDataValues()
{
	AvailableWarriorBeesToSpawn = SetupDataDefaultValue(AvailableWarriorBeesToSpawnTableName) + AvailableWarriorBeesToSpawnUpgrade;
	AvailableWorkerBeesToSpawn = SetupDataDefaultValue(AvailableWorkerBeesToSpawnTableName) + AvailableWorkerBeesToSpawnUpgrade;
	PollenCollectionCapacity = SetupDataDefaultValue(PollenCollectionCapacityTableName) + PollenCollectionCapacityUpgrade;
}

float AHomingMissileCharacter::SetupDataDefaultValue(const FName TableRowName) const
{
	if (AHomingMissileGameMode* GM = Cast<AHomingMissileGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		if (const FRealCurve* Curve = GM->RoundParamsCurveTable->FindCurve(TableRowName, FString::Printf(TEXT("%s"), *TableRowName.ToString())))
		{
			if (const AHomingMissileGameState* GS = Cast<AHomingMissileGameState>(UGameplayStatics::GetGameState(this)))
			{
				return Curve->Eval(GS->CurrentRound);
			}
		}
	}

	return 0.0f;
}
