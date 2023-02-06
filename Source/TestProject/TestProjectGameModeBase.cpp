// Copyright Epic Games, Inc. All Rights Reserved.


#include "TestProjectGameModeBase.h"
#include <Logging/LogMacros.h>
#include "TestGameState.h"


void ATestProjectGameModeBase::InitGameState()
{
	Super::InitGameState();

	CreateGrid();

	ATestGameState* GS = Cast<ATestGameState>(GameState);
	if (IsValid(GS))
	{
		GS->CreateTiles(gridSize);
		GS->CreateCharactersAtRandomPos(playerPositions);
	}
	float tickDuration = 0.1f;
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &ATestProjectGameModeBase::Tick, tickDuration, true, tickDuration);
}

void ATestProjectGameModeBase::CreateGrid()
{
	int32 Num = gridSize.X * gridSize.Y;
	grid.SetNum(Num);

	for (int32 y = 0; y < gridSize.X; ++y)
	{
		for (int32 x = 0; x < gridSize.Y; ++x)
		{
			int32 index = (y * gridSize.X) + x;
			FIntPoint pos = { x, y };
			grid[index].id = index;
			grid[index].pos = pos;
		}
	}
}

void ATestProjectGameModeBase::Tick()
{
	ATestGameState* GS = Cast<ATestGameState>(GameState);
	if (IsValid(GS))
	{
		GS->ServerTick(playerPositions);
	}
}
