// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"

#include "Collectable.generated.h"


UCLASS()
class SKATEPARK_API ACollectable : public AActor {
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent *NewRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent *VisualSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent *Collider;

public:
	ACollectable();

	// Called from Skater
	UFUNCTION()
	void Collect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Collectable", DisplayName = "StartCollectAnimation")
	void K2_StartCollectAnimation();
};
