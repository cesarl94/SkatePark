
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Skater.generated.h"


UCLASS()
class SKATEPARK_API ASkater : public ACharacter {
	GENERATED_BODY()

public:
	ASkater(const FObjectInitializer &ObjectInitializer);

	void OnLookInput(const FInputActionValue &Value);
	void OnSteerInput(const FInputActionValue &Value);
	void OnImpulseInput(const FInputActionValue &Value);
	void OnJumpInput(const FInputActionValue &Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Default values:

	// MappingContext for player input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputMappingContext *InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputAction *LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputAction *SteerAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputAction *ImpulseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputAction *JumpAction;

	// A multiplier to transform the delta of the mouse in pitch and yaw respectively
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float LookForce{3};

	// In centimeters per second
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float ImpulseMagnitude{1024};

	// Components:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent *CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent *Camera;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Skater", Meta = (CompactNodeTitle = "Movement Component"))
	class USkateMovementComponent *GetSkateMovementComponent() const;

	// Implementable Events:

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartJumpAnimation")
	void K2_StartJumpAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartImpulseAnimation")
	void K2_StartImpulseAnimation();
};
