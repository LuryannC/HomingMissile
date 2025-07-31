// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingMissile/Public/ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

