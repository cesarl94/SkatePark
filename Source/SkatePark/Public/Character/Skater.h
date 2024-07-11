
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Skater.generated.h"


UCLASS()
class SKATEPARK_API ASkater : public ACharacter {
	GENERATED_BODY()

private:
	void OnLookInput(const FInputActionValue &Value);
	void OnSteerInput(const FInputActionValue &Value);
	void OnImpulseInput(const FInputActionValue &Value);
	void OnJumpInput(const FInputActionValue &Value);

	void ComputeSteer(float DeltaTime);

	bool IsStopped{true};


protected:
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

	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float TurnForce{15};

	// In centimeters per second
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skate Defaults")
	float MinimumVelocityBeforeStop{100};

	// Components:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent *CameraSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent *Camera;

	// Private with blueprint access:

	UPROPERTY(BlueprintReadOnly, Category = "Skate")
	float SteeringValue;

public:
	ASkater(const FObjectInitializer &ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input. See https://unrealcommunity.wiki/using-the-enhancedinput-system-in-c++-74b72b
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// Components getters:

	UFUNCTION(BlueprintCallable, Category = "Skater|Components", Meta = (CompactNodeTitle = "Movement Component"))
	class USkateMovementComponent *GetSkateMovementComponent() const;

	// Implementable Events:

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartJumpAnimation")
	void K2_StartJumpAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartImpulseAnimation")
	void K2_StartImpulseAnimation();
};
