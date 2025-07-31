// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HomingMissileEntityBase.h"

#include "ActorComponents/HomingMissileTargetingComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AHomingMissileEntityBase::AHomingMissileEntityBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	HomingMissileTargetingComponent = CreateDefaultSubobject<UHomingMissileTargetingComponent>(TEXT("HomingMissileTargetingComponent"));
}

// Called when the game starts or when spawned
void AHomingMissileEntityBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomingMissileEntityBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

