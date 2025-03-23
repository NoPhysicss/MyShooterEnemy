// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyShooter/Cherecter/MyShooterHealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "MyShooter_StateEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MYSHOOTER_API UMyShooter_StateEffect : public UObject
{
	GENERATED_BODY()

public:

	virtual bool InitObject(AActor* Actor, FName NameBoneHit);
	virtual void DestroyObject();
	void LocationSave(FVector HitLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	TArray<TEnumAsByte<EPhysicalSurface>> PossibleInteractSurface;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	bool bIsStacable = false;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	//int MaxStackCout = 1;



	AActor* myActor = nullptr;

	FVector HitPoint;
};

UCLASS()
class MYSHOOTER_API UMyShooter_StateEffect_ExecuteOnce : public UMyShooter_StateEffect
{
	GENERATED_BODY()

public:

	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	void DestroyObject() override;

	virtual void ExecuteOnce();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 20.0f;

};

UCLASS()
class MYSHOOTER_API UMyShooter_StateEffect_ExecuteTimer : public UMyShooter_StateEffect
{
	GENERATED_BODY()

public:

	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	void DestroyObject() override;

	virtual void Execute();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Power = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Timer = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float RateTime = 1.0f;

	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;
};

UCLASS()
class MYSHOOTER_API UMyShooter_StateEffect_ExecuteTimerStatsIncomeDamage : public UMyShooter_StateEffect
{
	GENERATED_BODY()

public:

	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	void DestroyObject() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Status")
	float Timer = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Status")
	float CoefOfDamage;

	//костыль
	float tempmark = 0;

	FTimerHandle TimerHandle_EffectTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Status")
	UParticleSystem* ParticleEffect = nullptr;

	UParticleSystemComponent* ParticleEmitter = nullptr;
};

UCLASS()
class MYSHOOTER_API UMyShooter_StateEffect_ExecuteOnceRadial : public UMyShooter_StateEffect
{
	GENERATED_BODY()

public:

	bool InitObject(AActor* Actor, FName NameBoneHit) override;

	void DestroyObject() override;

	virtual void Execute();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float MinDamage = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float MaxDamage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float MinDamageRadius = 50.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float MaxDamageRadius = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float Timer = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	float RateTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	bool SelfDamage = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Once")
	bool PawnDamage = true;
	
	FTimerHandle TimerHandle_ExecuteTimer;
	FTimerHandle TimerHandle_EffectTimer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting Execute Status")
	UNiagaraSystem* NiagaraEffect = nullptr;
	
	UNiagaraComponent* NiagaraComponent = nullptr;

};