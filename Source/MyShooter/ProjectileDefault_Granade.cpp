// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDefault_Granade.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"

int32 DebugExplodeShow = 0;
FAutoConsoleVariableRef CVARExplodeShow(
	TEXT("MyShooter.DebugExplode"),
	DebugExplodeShow,
	TEXT("DrawDebug for Explode"),
	ECVF_Cheat
);

void AProjectileDefault_Granade::BeginPlay()
{
	Super::BeginPlay();


	ShowExplosionRadius();
}

void AProjectileDefault_Granade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimerExplose(DeltaTime);
	GranadeDecalTick(DeltaTime);
}

void AProjectileDefault_Granade::GranadeDecalTick(float DeltaTime)
{
	float ExpMinRadius = ProjectileSetting.ExploseMaxDamageSize;
	float ExpMaxRadius = ProjectileSetting.ExploseMinDamageSize;
	ExplosionMinRadiusDecal->DecalSize = FVector(ExpMinRadius, ExpMinRadius, ExpMinRadius);
	ExplosionMaxRadiusDecal->DecalSize = FVector(ExpMaxRadius, ExpMaxRadius, ExpMaxRadius);
}

void AProjectileDefault_Granade::TimerExplose(float DeltaTime)
{
	if (TimerEnabled)
	{
		if (TimerToExplose > TimeToExplose)
		{
			Explose();
		}
		else
		{
			TimerToExplose += DeltaTime;
		}
	}
}

void AProjectileDefault_Granade::BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::BulletCollisionSphereHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void AProjectileDefault_Granade::ImpactProjectile()
{
	TimerEnabled = true;
}

void AProjectileDefault_Granade::Explose()
{
	if (DebugExplodeShow)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ExploseMaxDamageSize, 12, FColor::Red, false, 12);
		DrawDebugSphere(GetWorld(), GetActorLocation(), ProjectileSetting.ExploseMinDamageSize, 12, FColor::Yellow, false, 12);
	}

	TimerEnabled = false;

	if (ProjectileSetting.ExploseFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploseFX, GetActorLocation(), GetActorRotation());
	}
	if (ProjectileSetting.ExploseSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ProjectileSetting.ExploseSound, GetActorLocation());
	}

	ExplosionMaxRadiusDecal->SetVisibility(false);
	ExplosionMinRadiusDecal->SetVisibility(false);

	TArray<AActor*>	IgnoreActor;
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
		ProjectileSetting.ExploseMaxDamage,
		ProjectileSetting.ExploseMinDamage,
		GetActorLocation(),
		ProjectileSetting.ExploseMaxDamageSize,
		ProjectileSetting.ExploseMinDamageSize,
		5,
		NULL, IgnoreActor, this, nullptr);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ProjectileSetting.ExploadedGround, FTransform(FRotator(0,0,0), this->GetActorLocation(), FVector(1.0f)));
	this->Destroy();

}

void AProjectileDefault_Granade::ShowExplosionRadius()
{
	DecalLocation = GetActorLocation();
	FRotator DecalRotation = FRotator(-90.f, 0.f, 0.f);


	ExplosionMinRadiusDecal->SetWorldLocationAndRotation(DecalLocation + (1.0f, 0.0f, 0.0f), DecalRotation);
	ExplosionMinRadiusDecal->SetVisibility(true);
	ExplosionMaxRadiusDecal->SetWorldLocationAndRotation(DecalLocation, DecalRotation);
	ExplosionMaxRadiusDecal->SetVisibility(true);
}
