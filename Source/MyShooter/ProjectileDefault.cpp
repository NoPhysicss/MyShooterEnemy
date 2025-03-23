#include "ProjectileDefault.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectileDefault::AProjectileDefault()
{


	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));

	BulletCollisionSphere->SetSphereRadius(16.f);
	
	
	BulletCollisionSphere->bReturnMaterialOnMove = true;//hit event return physMaterial

	BulletCollisionSphere->SetCanEverAffectNavigation(false);//collision not affect navigation (P keybord on editor)

	RootComponent = BulletCollisionSphere;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet Projectile Mesh"));
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Bullet FX"));
	BulletFX->SetupAttachment(RootComponent);

	//BulletSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Bullet Audio"));
	//BulletSound->SetupAttachment(RootComponent);

	BulletProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet ProjectileMovement"));
	BulletProjectileMovement->InitialSpeed = 1.f;
	BulletProjectileMovement->MaxSpeed = 0.f;

	BulletProjectileMovement->bRotationFollowsVelocity = true;
	BulletProjectileMovement->bShouldBounce = true;

	ExplosionMinRadiusDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ExplosionMinRadiusDecal"));
	ExplosionMinRadiusDecal->SetupAttachment(RootComponent);

	ExplosionMinRadiusDecal->SetVisibility(false);

	ExplosionMaxRadiusDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ExplosionMaxRadiusDecal"));
	ExplosionMaxRadiusDecal->SetupAttachment(RootComponent);

	ExplosionMaxRadiusDecal->SetVisibility(false);
}

// Called when the game starts or when spawned
void AProjectileDefault::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionSphere->OnComponentHit.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereHit);
	BulletCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereBeginOverlap);
	BulletCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AProjectileDefault::BulletCollisionSphereEndOverlap);


}

// Called every frame
void AProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DamageReductionTick(DeltaTime);

}

void AProjectileDefault::DamageReductionTick(float DeltaTime)
{
	if (ProjectileSetting.DamageReductionPerSecondByPersent > 0)
	{
		if (ProjectileSetting.ProjectileDamage > ProjectileSetting.ProjectileMinDamage)
		{
			if (FVector::Dist(ProjectileFirstPosition, GetActorLocation()) > ProjectileSetting.DamageReductionDistance)
			{
				if (ProjectileSetting.ProjectileMinDamage < ProjectileDamage)
				{
					ProjectileDamage -= DamageReduct;
				}
			}
		}
	}
}

void AProjectileDefault::InitProjectile(FProjectileInfo InitParam)
{
	BulletProjectileMovement->InitialSpeed = InitParam.ProjectileInitSpeed;
	BulletProjectileMovement->MaxSpeed = InitParam.ProjectileInitSpeed;
	this->SetLifeSpan(InitParam.ProjectileLifeTime);

	ProjectileSetting = InitParam;

	ProjectileFirstPosition = GetActorLocation();
	DamageReduct = ((ProjectileSetting.ProjectileDamage * ProjectileSetting.DamageReductionPerSecondByPersent) / 100.0f);
	ProjectileDamage = ProjectileSetting.ProjectileDamage;
	
}

void AProjectileDefault::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && Hit.PhysMaterial.IsValid())
	{
		EPhysicalSurface mySurfacetype = UGameplayStatics::GetSurfaceType(Hit);

		if (ProjectileSetting.HitDecals.Contains(mySurfacetype))
		{
			UMaterialInterface* myMaterial = ProjectileSetting.HitDecals[mySurfacetype];

			if (myMaterial && OtherComp)
			{
				UGameplayStatics::SpawnDecalAttached(myMaterial, FVector(ProjectileSetting.DecalSize), OtherComp, NAME_None, Hit.ImpactPoint, Hit.ImpactNormal.Rotation(), EAttachLocation::KeepWorldPosition, 10.0f);
			}
		}
		if (ProjectileSetting.HitFXs.Contains(mySurfacetype))
		{
			UParticleSystem* myParticle = ProjectileSetting.HitFXs[mySurfacetype];
			if (myParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), myParticle, FTransform(Hit.ImpactNormal.Rotation(), Hit.ImpactPoint, FVector(1.0f)));
			}
		}

		if (ProjectileSetting.HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ProjectileSetting.HitSound, Hit.ImpactPoint);
		}

		ULibTypes::AddEffectBySurfaceType(Hit.GetActor(),Hit.BoneName, ProjectileSetting.Effect, mySurfacetype, Hit.Location);
	}


	UGameplayStatics::ApplyDamage(OtherActor, ProjectileDamage, GetInstigatorController(), this, NULL);
	ImpactProjectile();
}

void AProjectileDefault::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileDefault::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AProjectileDefault::ImpactProjectile()
{
	this->Destroy();
}