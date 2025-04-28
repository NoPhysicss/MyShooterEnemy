// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponDefault.h"
#include "ProjectileDefault.h"
#include "EnemyWeaponComponente.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTER_API UEnemyWeaponComponente : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyWeaponComponente();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyWeapon")
	TSubclassOf<class AWeaponDefault> EnemyWeapon = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
