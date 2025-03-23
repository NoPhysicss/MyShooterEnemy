// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyShooter/Interface/MyShooter_IGameActor.h"
#include "Materials/MaterialInterface.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MyShooter/MyShooter_StateEffect.h"
#include "MyShooter_EnvironmentStructure.generated.h"

UCLASS()
class MYSHOOTER_API AMyShooter_EnvironmentStructure : public AActor, public IMyShooter_IGameActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyShooter_EnvironmentStructure();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//bool AviableForEffects_Implementation();

	//bool AviableForEffectsOnlyCPP() override;

	EPhysicalSurface GetSurfaceType() override;



	TArray<UMyShooter_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UMyShooter_StateEffect* RemovedEffect) override;
	void AddEffect(UMyShooter_StateEffect* NewEffect) override;
	//effect
	TArray<UMyShooter_StateEffect*> Effects;
};
