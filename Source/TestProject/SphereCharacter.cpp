// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereCharacter.h"
#include "Components/StaticMeshComponent.h"

ASphereCharacter::ASphereCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	meshComponent->SetCanEverAffectNavigation(false);
	meshComponent->SetSimulatePhysics(true);
	RootComponent = meshComponent;

	health = FMath::RandRange(2, 5);
	maxHealth = health;
}

void ASphereCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASphereCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASphereCharacter::SetTeamColor(ETeamColor color)
{
	UMaterialInterface* material = meshComponent->GetMaterial(0);
	if (IsValid(material))
	{
		if (!IsValid(materialInstance))
		{
			materialInstance = UMaterialInstanceDynamic::Create(material, this);
		}
		FLinearColor colorToSet = FLinearColor::Red;
		if (color == ETeamColor::Blue)
		{
			colorToSet = FLinearColor::Blue;
		}
		materialInstance->SetVectorParameterValue(FName(TEXT("Color")), colorToSet);
		meshComponent->SetMaterial(0, materialInstance);
	}
}

void ASphereCharacter::RecieveDamage()
{
	if (health > 0)
	{
		--health;
	}

	if (!IsValid(materialInstance))
	{
		UMaterialInterface* material = meshComponent->GetMaterial(0);
		materialInstance = UMaterialInstanceDynamic::Create(material, this);
	}
	materialInstance->SetScalarParameterValue(FName(TEXT("EmissionLevel")), 1 - ((float)health/maxHealth));
}
