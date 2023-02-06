// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSpectatorPawn.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpectatorPawnMovement.h"
#include "GameFramework/WorldSettings.h"

ATestSpectatorPawn::ATestSpectatorPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<USpectatorPawnMovement>(Super::MovementComponentName)
		.DoNotCreateDefaultSubobject(Super::MeshComponentName)
	)
{
	SetCanBeDamaged(false);
	//SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	//bReplicates = true;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	MovementComponent->bComponentShouldUpdatePhysicsVolume = false;

	static FName CollisionProfileName(TEXT("Spectator"));
	GetCollisionComponent()->SetCollisionProfileName(CollisionProfileName);

	UFloatingPawnMovement* floatingMovement = Cast<UFloatingPawnMovement>(MovementComponent);
	if (IsValid(floatingMovement))
	{
		floatingMovement->MaxSpeed = 10000;
	}
}


void ATestSpectatorPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATestSpectatorPawn::PossessedBy(class AController* NewController)
{
	if (bReplicates)
	{
		Super::PossessedBy(NewController);
	}
	else
	{
		// We don't want the automatic changing of net role in Pawn code since we don't replicate, so don't call Super.
		AController* const OldController = Controller;
		Controller = NewController;

		// dispatch Blueprint event if necessary
		if (OldController != NewController)
		{
			ReceivePossessed(Controller);
		}
	}
}

void ATestSpectatorPawn::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
		}
	}
}

void ATestSpectatorPawn::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
		}
	}
}

void ATestSpectatorPawn::TurnAtRate(float Rate)
{
	// Replays that use small or zero time dilation to pause will block gamepads from steering the spectator pawn
	AWorldSettings* const WorldSettings = GetWorldSettings();
	if (WorldSettings)
	{
		float TimeDilation = WorldSettings->GetEffectiveTimeDilation();
		if (TimeDilation <= KINDA_SMALL_NUMBER)
		{
			const float DeltaTime = FApp::GetDeltaTime();
			AddControllerYawInput(Rate * BaseTurnRate * DeltaTime * CustomTimeDilation);
			return;
		}
	}

	Super::TurnAtRate(Rate);
}

void ATestSpectatorPawn::LookUpAtRate(float Rate)
{
	// Replays that use small or zero time dilation to pause will block gamepads from steering the spectator pawn
	AWorldSettings* const WorldSettings = GetWorldSettings();
	if (WorldSettings)
	{
		float TimeDilation = WorldSettings->GetEffectiveTimeDilation();
		if (TimeDilation <= KINDA_SMALL_NUMBER)
		{
			const float DeltaTime = FApp::GetDeltaTime();
			AddControllerPitchInput(Rate * BaseLookUpRate * DeltaTime * CustomTimeDilation);
			return;
		}
	}

	Super::LookUpAtRate(Rate);
}
