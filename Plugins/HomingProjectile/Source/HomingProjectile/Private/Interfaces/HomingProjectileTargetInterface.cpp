#include "Interfaces/HomingProjectileTargetInterface.h"

bool IHomingProjectileTargetInterface::CanBeTargeted_Implementation()
{
	return false;
}

bool IHomingProjectileTargetInterface::CanTarget_Implementation(AActor* InTarget)
{
	return true;
}

FVector IHomingProjectileTargetInterface::GetFiringSpawnLocation_Implementation()
{
	return FVector::ZeroVector;
}

void IHomingProjectileTargetInterface::FireProjectile_Implementation(AActor* Target)
{
}