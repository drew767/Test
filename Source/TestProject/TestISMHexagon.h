// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestISMHexagon.generated.h"

UCLASS()
class TESTPROJECT_API ATestISMHexagon : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestISMHexagon();
	FVector3d GetRandomTilePosition(FIntPoint& outParam);
	FVector3d GetTilePosition(const FIntPoint& offsetPosition);
	void CreateGrid(FIntPoint& gridSize);
	FVector3d PositionToWorldOffset(int32 x, int32 y);
	FIntPoint OffsetToAxial(const FIntPoint& offset);
	FVector2d OffsetToAxialFloat(const FIntPoint& offset);
	FIntPoint AxialToOffset(FIntPoint& axial);
	int32 GetOffsetDistance(FIntPoint& a, const FIntPoint& b);
	int32 GetAxialDistance(FVector2d& a, const FVector2d& b);
	void InitAdditionalHexInfo();
	bool GetNearestFromAToB(FIntPoint& a, FIntPoint& b);
	FIntPoint AxialAdd(FIntPoint& a, FIntPoint& b);

public:
	UPROPERTY(VisibleAnywhere)
		class UInstancedStaticMeshComponent* ISMHexgaon;
	FIntPoint gridSize;
	TArray<FIntPoint> HexDirections;
};
