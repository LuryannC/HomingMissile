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

uint8 IHomingProjectileInterface::GetProjectileTeam_Implementation()
{
	return 0;
}


FVector IHomingProjectileInterface::GetFiringSpawnLocation_Implementation()
{
	return FVector::ZeroVector;
}

void IHomingProjectileInterface::FireProjectile_Implementation(AActor* Target)
{
}