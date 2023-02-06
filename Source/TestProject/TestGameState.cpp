// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameState.h"
#include "TestISMHexagon.h"

void ATestGameState::CreateTiles(FIntPoint& incomingGridSize)
{
	FTransform spawnTM;
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnTM.SetLocation(FVector3d(0.0f));
	hexagonMap = GetWorld()->SpawnActor<ATestISMHexagon>(tilesMapClass, spawnTM, spawnParams);

	if (IsValid(hexagonMap))
	{
		hexagonMap->CreateGrid(incomingGridSize);
	}
}

void ATestGameState::CreateCharactersAtRandomPos(TArray<FIntPoint>& playerPositions)
{
	playerPositions.SetNum(2);
	FIntPoint pos;
	characters.Add(CreateCharacter(ETeamColor::Blue, pos));
	playerPositions[0] = pos;
	characters.Add(CreateCharacter(ETeamColor::Red, pos));
	playerPositions[1] = pos;
}

void ATestGameState::ServerTick(TArray<FIntPoint>& playerPositions)
{
	bool moved = MakeMove(playerPositions);
	if (moved)
	{
		UpdatePositions(playerPositions);
	}
	else
	{
		Fight();
	}
}

bool ATestGameState::MakeMove(TArray<FIntPoint>& playerPositions)
{
	bool result = hexagonMap->GetNearestFromAToB(playerPositions[0], playerPositions[1]);
	result = hexagonMap->GetNearestFromAToB(playerPositions[1], playerPositions[0]) || result;
	return result;
}

ASphereCharacter* ATestGameState::CreateCharacter(ETeamColor team, FIntPoint& out)
{
	FTransform spawnTM;
	FActorSpawnParameters spawnParams;
	FVector3d positionToSet = hexagonMap->GetRandomTilePosition(out);
	spawnTM.SetLocation(positionToSet);
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ASphereCharacter* result = GetWorld()->SpawnActor<ASphereCharacter>(characterClass, spawnTM, spawnParams);
	result->SetTeamColor(team);
	return result;
}

void ATestGameState::Fight()
{
	if (characters[0]->IsAlive())
	{
		characters[1]->RecieveDamage();
	}

	if (characters[1]->IsAlive())
	{
		characters[0]->RecieveDamage();
	}
}

void ATestGameState::UpdatePositions(TArray<FIntPoint>& playerPositions)
{
	for (int32 i = 0; i < characters.Num(); ++i)
	{
		characters[i]->SetActorLocation(hexagonMap->GetTilePosition(playerPositions[i]));
	}
}

