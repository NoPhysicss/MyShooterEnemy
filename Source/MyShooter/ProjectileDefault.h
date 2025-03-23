#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MyShooter/MyShooter_StateEffect.h"
#include "GameFramework/ProjectileMovementComponent.h"


#include "MyShooter/FuncLibrary/LibTypes.h"
#include "ProjectileDefault.generated.h"

UCLASS()
class MYSHOOTER_API AProjectileDefault : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileDefault();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UStaticMeshComponent* BulletMesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class USphereComponent* BulletCollisionSphere = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UProjectileMovementComponent* BulletProjectileMovement = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UParticleSystemComponent* BulletFX = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UDecalComponent* ExplosionMinRadiusDecal = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Components)
	class UDecalComponent* ExplosionMaxRadiusDecal = nullptr;
	UPROPERTY(BlueprintReadOnly)
	FProjectileInfo ProjectileSetting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DamageReductionTick(float DeltaTime);

	void InitProjectile(FProjectileInfo InitParam);

	UFUNCTION()
	virtual void ImpactProjectile();

	//костыли
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ProjectileDamage = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageReduct = 0.0f;
	FVector ProjectileFirstPosition;


	UFUNCTION(BlueprintCallable)
	void BulletCollisionSphereHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
