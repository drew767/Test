// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SphereCharacter.generated.h"

UENUM()
enum class ETeamColor : uint8
{
	Blue     UMETA(DisplayName = "Blue"),
	Red      UMETA(DisplayName = "Red")
};

UCLASS()
class TESTPROJECT_API ASphereCharacter : public APawn
{
	GENERATED_BODY()

public:
	ASphereCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetTeamColor(ETeamColor color);
	void RecieveDamage();
	bool IsAlive() { return health > 0; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
		class UStaticMeshComponent* meshComponent;
	UMaterialInstanceDynamic* materialInstance;

public: 
	int32 maxHealth;
	int32 health;
};
