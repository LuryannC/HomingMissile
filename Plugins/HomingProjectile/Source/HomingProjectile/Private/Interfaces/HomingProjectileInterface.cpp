#include "Interfaces/HomingProjectileInterface.h"

bool IHomingProjectileInterface::CanBeTargeted_Implementation()
{
	return false;
}

void IHomingProjectileInterface::SetProjectileTarget_Implementation(AActor* Target)
{
}

AActor* IHomingProjectileInterface::GetProjectileTarget_Implementation()
{
	return nullptr;
}

FVector IHomingProjectileInterface::GetFiringSpawnLocation_Implementation()
{
	return FVector::ZeroVector;
}

void IHomingProjectileInterface::FireProjectile_Implementation(AActor* Target)
{
}