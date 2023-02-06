// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestProjectGameModeBase.generated.h"

USTRUCT(BlueprintType)
struct FHexagon
{
	GENERATED_BODY()
public:
	int32 id;
	FIntPoint pos;
};

/**
 * 
 */
UCLASS()
class TESTPROJECT_API ATestProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void InitGameState() override;

protected:
	void CreateGrid();
	
	UFUNCTION()
		void Tick();

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FHexagon> grid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint gridSize;
	TArray<FIntPoint> playerPositions;

	FTimerHandle TickTimerHandle;
};
