// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Skater.h"

// Sets default values
ASkater::ASkater()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASkater::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

