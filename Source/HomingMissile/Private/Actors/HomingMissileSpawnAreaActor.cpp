// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HomingMissileSpawnAreaActor.h"

#include "Components/BoxComponent.h"


// Sets default values
AHomingMissileSpawnAreaActor::AHomingMissileSpawnAreaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxComponent);
}