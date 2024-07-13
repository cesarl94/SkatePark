#include "Character/Skater.h"

#include "Actors/Collectable.h"
#include "Camera/CameraComponent.h"
#include "Character/SkateMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/Rotator.h"
#include "Math/Vector2D.h"
#include "Utils/MathUtils.h"

void ASkater::OnCapsuleBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult) {
	if (ACollectable *Collectable = Cast<ACollectable>(OtherActor)) {
		Collectable->Collect();
		CollectablesCount++;
		OnCollectCollectable.Broadcast(CollectablesCount);
	}
}

void ASkater::OnLookInput(const FInputActionValue &Value) {
	FVector2D Axis2DValue = Value.Get<FVector2D>();

	CameraSpringArm->AddRelativeRotation(FRotator(Axis2DValue.Y * LookForce, Axis2DValue.X * LookForce, 0));
}

void ASkater::OnSteerInput(const FInputActionValue &Value) {
	float FloatValue = Value.Get<float>();

	SteeringValue = FloatValue;
}

void ASkater::OnImpulseInput(const FInputActionValue &Value) {
	bool IsPressed = Value.Get<bool>();

	if (!IsPressed) {
		return;
	}

	USkateMovementComponent *SkateMovementComponent = GetSkateMovementComponent();

	if (!SkateMovementComponent->IsFalling()) {
		FVector Forward = GetActorForwardVector();
		SkateMovementComponent->AddImpulse(Forward * ImpulseMagnitude, true);


		// We trigger the animation from blueprints
		K2_StartImpulseAnimation();
		IsStopped = false;
	}
}

void ASkater::OnJumpInput(const FInputActionValue &Value) {
	bool IsPressed = Value.Get<bool>();

	if (!IsPressed) {
		return;
	}

	// We trigger the animation from blueprints.
	// This should internally trigger an event when you need to apply the jump impulse.
	// The AnimationBlueprint will encharge of this.
	K2_StartJumpAnimation();
}

void ASkater::OnStopInput(const FInputActionValue &Value) {
	IsTryingToStop = Value.Get<bool>();

	if (IsTryingToStop && !GetSkateMovementComponent()->IsFalling()) {
		if (IsStopped) {
			FVector Forward = GetActorForwardVector();

			GetSkateMovementComponent()->AddImpulse(Forward * BackflipImpulse, true);

			// We trigger the animation from blueprints
			K2_StartBackflipAnimation();
		} else {
			K2_StartStopAnimation();
		}
	}
}

void ASkater::ComputeSteer(float DeltaTime) {
	USkateMovementComponent *SkateMovementComponent = GetSkateMovementComponent();

	// Check if the SteeringValue is non-zero
	if (!SkateMovementComponent->IsFalling() && FMath::Abs(SteeringValue) > KINDA_SMALL_NUMBER) {
		float VelocityMagnitude = SkateMovementComponent->Velocity.Size();

		if (VelocityMagnitude > MinimumVelocityBeforeStop) {
			float SpeedRatio = VelocityMagnitude / GetSkateMovementComponent()->GetMaxSpeed();
			float SteerForceInterpolated = FMath::Lerp(SteerForceAtSpeedRatio0, SteerForceAtSpeedRatio1, SpeedRatio);
			float RotationDelta = SteeringValue * DeltaTime * SteerForceInterpolated;

			AddControllerYawInput(RotationDelta);
			SkateMovementComponent->Velocity = GetActorForwardVector() * VelocityMagnitude;
		} else if (IsStuck) {
			AddControllerYawInput(SteeringValue * DeltaTime * SteerForceWhenYouAreStuck);
		}
	}
}

void ASkater::ComputeStop(float DeltaTime) {
	USkateMovementComponent *SkateMovementComponent = GetSkateMovementComponent();
	float StopSmoothLerpAlpha = UMathUtils::GetSmoothLerpAlpha(DeltaTime, StopForceHalfLife);

	float VelocitySqrMagnitude = SkateMovementComponent->Velocity.SizeSquared();
	bool IsVelocityTooSmall = VelocitySqrMagnitude < MinimumVelocityBeforeStop * MinimumVelocityBeforeStop;
	if (IsVelocityTooSmall) {
		IsStopped = true;
	}
	if (IsVelocityTooSmall || (IsTryingToStop && !SkateMovementComponent->IsFalling())) {
		// We apply an extra break when the velocity is small
		SkateMovementComponent->Velocity = FMath::Lerp(FVector::ZeroVector, SkateMovementComponent->Velocity, StopSmoothLerpAlpha);
	}
}

void ASkater::CheckMaxSpeed() {
	USkateMovementComponent *SkateMovementComponent = GetSkateMovementComponent();

	float MaxSpeed = SkateMovementComponent->GetMaxSpeed();
	float CurrentVelocitySquared2D = SkateMovementComponent->Velocity.SizeSquared2D();

	if (CurrentVelocitySquared2D > MaxSpeed * MaxSpeed * 1.01f) {
		FVector VelocityDirection2D = SkateMovementComponent->Velocity.GetSafeNormal2D();
		FVector NewVelocity2D = VelocityDirection2D * MaxSpeed;

		SkateMovementComponent->Velocity = NewVelocity2D;
	}
}

void ASkater::CheckStuck() {
	// Get the character's location and direction
	FVector StartLocation = GetActorLocation() - FVector(0, 0, GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 0.5f);
	FVector ForwardVector = GetActorForwardVector();
	FVector LeftVector = ForwardVector.RotateAngleAxis(-20.0f, FVector(0, 0, 1));
	FVector RightVector = ForwardVector.RotateAngleAxis(20.0f, FVector(0, 0, 1));

	FVector EndLocationForward = StartLocation + (ForwardVector * CheckStuckRayDistance);
	FVector EndLocationLeft = StartLocation + (LeftVector * CheckStuckRayDistance);
	FVector EndLocationRight = StartLocation + (RightVector * CheckStuckRayDistance);

	// Define collision parameters
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the character itself

	// Perform the ray trace
	FHitResult OutHit;
	bool bHitForward = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocationForward, ECC_Visibility, CollisionParams);
	bool bHitLeft = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocationLeft, ECC_Visibility, CollisionParams);
	bool bHitRight = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocationRight, ECC_Visibility, CollisionParams);

	// Update the boolean IsStuck based on the results of the ray traces
	IsStuck = bHitForward || bHitLeft || bHitRight;
}

void ASkater::BeginPlay() {
	Super::BeginPlay();

	StopForceHalfLife = UMathUtils::CalculateSmoothLerpHalfLife(60, StopForce);
}

ASkater::ASkater(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USkateMovementComponent>(CharacterMovementComponentName)) {
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASkater::OnCapsuleBeginOverlap);
}

void ASkater::Tick(float DeltaTime) {
	CheckMaxSpeed();
	CheckStuck();

	ComputeSteer(DeltaTime);
	ComputeStop(DeltaTime);


	Super::Tick(DeltaTime);
}

// Called to bind functionality to input. See https://unrealcommunity.wiki/using-the-enhancedinput-system-in-c++-74b72b
void ASkater::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
	APlayerController *PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkater::OnLookInput);
	Input->BindAction(ImpulseAction, ETriggerEvent::Triggered, this, &ASkater::OnImpulseInput);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkater::OnJumpInput);
	Input->BindAction(SteerAction, ETriggerEvent::Completed, this, &ASkater::OnSteerInput);
	Input->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASkater::OnSteerInput);
	Input->BindAction(StopAction, ETriggerEvent::Triggered, this, &ASkater::OnStopInput);
}

int32 ASkater::GetCollectablesCount() const { return CollectablesCount; }

USkateMovementComponent *ASkater::GetSkateMovementComponent() const { return Cast<USkateMovementComponent>(GetCharacterMovement()); }
