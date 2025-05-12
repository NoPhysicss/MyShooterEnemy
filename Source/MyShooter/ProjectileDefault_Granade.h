// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyShooter/ProjectileDefault.h"
#include "ProjectileDefault_Granade.generated.h"



/**
 * 
 */
UCLASS()
class MYSHOOTER_API AProjectileDefault_Granade : public AProjectileDefault
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	void GranadeDecalTick(float DeltaTime);

	void TimerExplose(float DeltaTime);

	virtual void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	virtual void ImpactProjectile() override;

	void Explose();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	bool TimerEnabled = false;
	float TimerToExplose = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade")
	float TimeToExplose = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	UDecalComponent* ExploseDecal = nullptr;

	void ShowExplosionRadius();

	FVector DecalLocation;


};
