// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SphereCharacter.h"
#include "TestGameState.generated.h"

/**
 * 
 */

UCLASS()
class TESTPROJECT_API ATestGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void CreateTiles(FIntPoint& gridSize);
	void CreateCharactersAtRandomPos(TArray<FIntPoint>& playerPositions);
	void ServerTick(TArray<FIntPoint>& playerPositions);
	bool MakeMove(TArray<FIntPoint>& playerPositions);

protected:
	ASphereCharacter* CreateCharacter(ETeamColor team, FIntPoint& out);
	void Fight();
	void UpdatePositions(TArray<FIntPoint>& playerPositions);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actors")
		TSubclassOf<class ATestISMHexagon> tilesMapClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Actors")
		TSubclassOf<ASphereCharacter> characterClass;

	ATestISMHexagon* hexagonMap;
	TArray<ASphereCharacter*> characters;
};
