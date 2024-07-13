// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

#include "SkateMovementComponent.generated.h"

// I've used this class but I can't use it now because the component data in the Blueprint causes corruption :)
// I have few time to worry about fixing this.

UCLASS()
class SKATEPARK_API USkateMovementComponent : public UCharacterMovementComponent {
	GENERATED_BODY()
};
