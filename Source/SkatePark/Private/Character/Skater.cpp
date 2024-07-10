#include "Character/Skater.h"

#include "Camera/CameraComponent.h"
#include "Character/SkateMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Math/Rotator.h"
#include "Math/Vector2D.h"


// Sets default values
ASkater::ASkater(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USkateMovementComponent>(CharacterMovementComponentName)) {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm);
}

void ASkater::OnLookInput(const FInputActionValue &Value) {
	FVector2D Axis2DValue = Value.Get<FVector2D>();

	CameraSpringArm->AddRelativeRotation(FRotator(Axis2DValue.Y * LookForce, Axis2DValue.X * LookForce, 0));
}

void ASkater::OnSteerInput(const FInputActionValue &Value) {}

void ASkater::OnImpulseInput(const FInputActionValue &Value) {
	bool IsPressed = Value.Get<bool>();

	if (!IsPressed) {
		return;
	}

	USkateMovementComponent *SkateMovementComponent = GetSkateMovementComponent();

	if (SkateMovementComponent->MovementMode == MOVE_Walking) {
		FVector Forward = GetActorForwardVector();
		SkateMovementComponent->AddImpulse(Forward * ImpulseMagnitude, true);

		// We trigger the animation from blueprints
		K2_StartImpulseAnimation();

		float MaxSpeed = SkateMovementComponent->GetMaxSpeed();
		if (SkateMovementComponent->IsExceedingMaxSpeed(MaxSpeed)) {
			FVector VelocityDirection2D = SkateMovementComponent->Velocity.GetSafeNormal2D();
			FVector NewVelocity2D = VelocityDirection2D * MaxSpeed;

			SkateMovementComponent->Velocity = NewVelocity2D;
		}
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


// Called when the game starts or when spawned
void ASkater::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ASkater::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

// Called to bind functionality to input. See https://unrealcommunity.wiki/using-the-enhancedinput-system-in-c++-74b72b
void ASkater::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) {
	APlayerController *PlayerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent *Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASkater::OnLookInput);
	Input->BindAction(SteerAction, ETriggerEvent::Triggered, this, &ASkater::OnSteerInput);
	Input->BindAction(ImpulseAction, ETriggerEvent::Triggered, this, &ASkater::OnImpulseInput);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASkater::OnJumpInput);
}

USkateMovementComponent *ASkater::GetSkateMovementComponent() const { return Cast<USkateMovementComponent>(GetCharacterMovement()); }
