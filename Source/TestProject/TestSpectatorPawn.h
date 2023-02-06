// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/DefaultPawn.h"
#include "TestSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS(config = Game, Blueprintable, BlueprintType, notplaceable)
class TESTPROJECT_API ATestSpectatorPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	ATestSpectatorPawn(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	// Begin Pawn overrides
	/** Overridden to avoid changing network role. If subclasses want networked behavior, call the Pawn::PossessedBy() instead. */
	virtual void PossessedBy(class AController* NewController) override;
	virtual void MoveRight(float Val) override;
	virtual void MoveForward(float Val) override;
	virtual void TurnAtRate(float Rate) override;
	virtual void LookUpAtRate(float Rate) override;
	// End Pawn overrides
};
