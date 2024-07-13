
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "Skater.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectCollectable, int32, CollectablesCount);

UCLASS()
class SKATEPARK_API ASkater : public ACharacter {
	GENERATED_BODY()

private:
	bool IsStopped{true};
	// Related to the Input
	bool IsTryingToStop{false};
	// Used for the SmoothLerp
	float StopForceHalfLife;

	int32 CollectablesCount{0};

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	void OnLookInput(const FInputActionValue &Value);
	void OnSteerInput(const FInputActionValue &Value);
	void OnImpulseInput(const FInputActionValue &Value);
	void OnJumpInput(const FInputActionValue &Value);
	void OnStopInput(const FInputActionValue &Value);

	// Process the steering behaviour
	void ComputeSteer(float DeltaTime);

	void ComputeStop(float DeltaTime);

	void CheckMaxSpeed();


protected:
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inputs")
	class UInputAction *StopAction;

	// A multiplier to transform the delta of the mouse in pitch and yaw respectively
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float LookForce{3};

	// In centimeters per second
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float ImpulseMagnitude{1024};

	// In centimeters per second
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float BackflipImpulse{-250};

	// This value is to balance the steering force
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float SteerForce{15};

	// Used to reduce speed with a SmoothLerp. If it is 1, the speed will remain the same; if it is 0, it will stop immediately.
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float StopForce{0.9};

	// This is the initial velocity in Z when we jump. Try to avoid put a value that allow you to leave from safe zone of the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skate Defaults")
	float JumpZForce{900};

	// This is the initial velocity in Z when we make a backflip. Try to avoid put a value higher to JumpZForce to avoid reach high places
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skate Defaults")
	float BackflipZForce{400};

	// In centimeters per second. If you're going in a lower velocity, you can be consider stopped
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skate Defaults")
	float MinimumVelocityBeforeStop{100};

	// Steering force applied when speed is 0
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float SteerForceAtSpeedRatio0{30};

	// Steering force applied when speed is at maximum
	UPROPERTY(EditDefaultsOnly, Category = "Skate Defaults")
	float SteerForceAtSpeedRatio1{15};

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

	UFUNCTION(BlueprintCallable, Category = "Skater", Meta = (CompactNodeTitle = "Collectables Count"))
	int32 GetCollectablesCount() const;

	// Components getters:

	UFUNCTION(BlueprintCallable, Category = "Skater|Components", Meta = (CompactNodeTitle = "Movement Component"))
	class USkateMovementComponent *GetSkateMovementComponent() const;

	// Implementable Events:

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartJumpAnimation")
	void K2_StartJumpAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartImpulseAnimation")
	void K2_StartImpulseAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartStopAnimation")
	void K2_StartStopAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "Skater", DisplayName = "StartBackflipAnimation")
	void K2_StartBackflipAnimation();

	// Events:

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, Category = "Skater|Events")
	FOnCollectCollectable OnCollectCollectable;
};
