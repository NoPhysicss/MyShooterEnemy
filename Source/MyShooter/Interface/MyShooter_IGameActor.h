// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyShooter/MyShooter_StateEffect.h"
#include "MyShooter/FuncLibrary/LibTypes.h"
#include "MyShooter_IGameActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyShooter_IGameActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYSHOOTER_API IMyShooter_IGameActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	//UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Interface")
	//void AviableForEffectsBP();
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	//bool AviableForEffects();



	virtual EPhysicalSurface GetSurfaceType();

	TArray<UMyShooter_StateEffect*> AllCurrentEffects;

	virtual TArray<UMyShooter_StateEffect*> GetAllCurrentEffects();
	virtual void RemoveEffect(UMyShooter_StateEffect* RemovedEffect);
	virtual void AddEffect(UMyShooter_StateEffect* NewEffect);

	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void DropWeaponToWord(FDropItem DropItemInfo);
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	//void DropAmmoToWorld(EWeaponType TypeAmmo, int32 cout);
};
