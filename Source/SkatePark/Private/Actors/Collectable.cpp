// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Collectable.h"

#include "Components/SphereComponent.h"


ACollectable::ACollectable() {
	PrimaryActorTick.bCanEverTick = true;

	NewRoot = CreateDefaultSubobject<USceneComponent>(TEXT("New Root"));
	SetRootComponent(NewRoot);

	VisualSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Visual Scene Component"));
	VisualSceneComponent->SetupAttachment(NewRoot);

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(NewRoot);
}

// void ACollectable::BeginPlay()
// {
// 	Super::BeginPlay();

// }

// void ACollectable::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);

// }

void ACollectable::Collect() {
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	K2_StartCollectAnimation();
}
