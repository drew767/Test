// Fill out your copyright notice in the Description page of Project Settings.
#include "TestISMHexagon.h"

#include "Math/NumericLimits.h"
#include "DrawDebugHelpers.h"
#include "Components/InstancedStaticMeshComponent.h"

ATestISMHexagon::ATestISMHexagon()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	ISMHexgaon = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISMComponent"));
	SetRootComponent(ISMHexgaon);
	ISMHexgaon->SetMobility(EComponentMobility::Static);
	ISMHexgaon->SetCollisionProfileName("BlockAll");
}

FVector3d ATestISMHexagon::GetRandomTilePosition(FIntPoint& outParam)
{
	outParam.X = FMath::RandRange(0, gridSize.X - 1);
	outParam.Y = FMath::RandRange(0, gridSize.Y - 1);
	return PositionToWorldOffset(outParam.X, outParam.Y);
}

FVector3d ATestISMHexagon::GetTilePosition(const FIntPoint& offsetPosition)
{
	return PositionToWorldOffset(offsetPosition.X, offsetPosition.Y);
}

void ATestISMHexagon::CreateGrid(FIntPoint& incomingGridSize)
{
	gridSize = incomingGridSize;
	int32 Num = gridSize.X * gridSize.Y;

	for (int32 j = 0; j < gridSize.X; ++j)
	{
		for (int32 i = 0; i < gridSize.Y; ++i)
		{
			FTransform RelativeZeroPoint = GetTransform();
			FVector3d positionToSet = PositionToWorldOffset(i, j);
			positionToSet.Z = 0;
			RelativeZeroPoint.AddToTranslation(positionToSet);
			ISMHexgaon->AddInstance(RelativeZeroPoint);
		}
	}

	InitAdditionalHexInfo();
}

FVector3d ATestISMHexagon::PositionToWorldOffset(int32 x, int32 y)
{
	FVector size = ISMHexgaon->GetStaticMesh()->GetBounds().GetBox().GetSize();

	double rowOffset = (y % 2) * (size.X * 0.5f);
	FVector3d result;
	result.X = x * size.X + rowOffset;
	result.Y = y * (size.Y * 0.75f);
	return result;
}

FIntPoint ATestISMHexagon::OffsetToAxial(const FIntPoint& offset)
{
	FIntPoint result;
	result.X = offset.X + (offset.Y - (offset.Y & 1)) / 2;
	result.Y = offset.Y;
	return result;
}

FVector2d ATestISMHexagon::OffsetToAxialFloat(const FIntPoint& offset)
{
	FVector2d result;
	result.X = offset.X + (offset.Y - (offset.Y & 1)) / 2.0f;
	result.Y = offset.Y;
	return result;
}

int32 ATestISMHexagon::GetOffsetDistance(FIntPoint& a, const FIntPoint& b)
{
	FVector2d axialA = OffsetToAxialFloat(a);
	FVector2d axialB = OffsetToAxialFloat(b);

	return GetAxialDistance(axialA, axialB);
}

int32 ATestISMHexagon::GetAxialDistance(FVector2d& a, const FVector2d& b)
{
	FVector2d vec = a - b;
	return (FMath::Abs(vec.X) + FMath::Abs(vec.X + vec.Y) + FMath::Abs(vec.Y)) / 2.f;
}

void ATestISMHexagon::InitAdditionalHexInfo()
{
	// even rows 
	HexDirections.Add(FIntPoint(1, 0));
	HexDirections.Add(FIntPoint(0, -1));
	HexDirections.Add(FIntPoint(-1, -1));
	HexDirections.Add(FIntPoint(-1, 0));
	HexDirections.Add(FIntPoint(-1, 1));
	HexDirections.Add(FIntPoint(0, 1));

	// odd rows 
	HexDirections.Add(FIntPoint(1, 0));
	HexDirections.Add(FIntPoint(1, -1));
	HexDirections.Add(FIntPoint(0, -1));
	HexDirections.Add(FIntPoint(-1, 0));
	HexDirections.Add(FIntPoint(0, 1));
	HexDirections.Add(FIntPoint(1, 1));
}

bool ATestISMHexagon::GetNearestFromAToB(FIntPoint& a, FIntPoint& b)
{
	int32 minimalDistance = GetOffsetDistance(a, b);
	FIntPoint minimalHex;
	FIntPoint currentHex;
	bool res = false;

	int32 begin, end;
	if (a.Y % 2 == 0)
	{
		begin = 0;
		end = 6;
	}
	else
	{
		begin = 6;
		end = 12;
	}

	for (int32 i = begin; i < end; ++i)
	{
		currentHex.X = a.X + HexDirections[i].X;
		currentHex.Y = a.Y + HexDirections[i].Y;
		if (b.X == currentHex.X && b.Y == currentHex.Y)
		{
			return false;
		}

		if (currentHex.X < 0 || 
			currentHex.X >= gridSize.X ||
			currentHex.Y < 0 ||
			currentHex.Y >= gridSize.Y)
		{
			continue;
		}

		int32 distance = GetOffsetDistance(currentHex, b);
		if (distance < minimalDistance)
		{
			minimalHex = currentHex;
			minimalDistance = distance;
			res = true;
		}
	}

	if (res)
	{
		a = minimalHex;
	}
	return res;
}

FIntPoint ATestISMHexagon::AxialAdd(FIntPoint& a, FIntPoint& b)
{
	return FIntPoint(a.X + b.X, a.Y + b.Y);
}
