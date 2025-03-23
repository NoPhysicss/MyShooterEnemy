// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShooter_StateEffect.h"
#include "Kismet/GameplayStatics.h"
#include "MyShooter/Interface/MyShooter_IGameActor.h"
#include "MyShooter/Cherecter/MyShooterHealthComponent.h"

bool UMyShooter_StateEffect::InitObject(AActor* Actor, FName NameBoneHit)
{
	myActor = Actor;

	IMyShooter_IGameActor* myInterface = Cast<IMyShooter_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->AddEffect(this);
	}

	return true;
}

void UMyShooter_StateEffect::LocationSave(FVector HitLocation)
{
	HitPoint = HitLocation;
}

void UMyShooter_StateEffect::DestroyObject()
{
	IMyShooter_IGameActor* myInterface = Cast<IMyShooter_IGameActor>(myActor);
	if (myInterface)
	{
		myInterface->RemoveEffect(this);
	}

	myActor = nullptr;

	if (this && this->IsValidLowLevel())
	{
		this->ConditionalBeginDestroy();
	}
	HitPoint = FVector(0);
}



bool UMyShooter_StateEffect_ExecuteOnce::InitObject(AActor* Actor, FName NameBoneHit)
{
	Super::InitObject(Actor, NameBoneHit);

	ExecuteOnce();

	return true;
}

void UMyShooter_StateEffect_ExecuteOnce::DestroyObject()
{
	Super::DestroyObject();
}

void UMyShooter_StateEffect_ExecuteOnce::ExecuteOnce()
{
	if (myActor)
	{
		UMyShooterHealthComponent* myHealthComp = Cast<UMyShooterHealthComponent>(myActor->GetComponentByClass(UMyShooterHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeCurrentHealt(Power);
		}
	}
	

	DestroyObject();
}






bool UMyShooter_StateEffect_ExecuteTimer::InitObject(AActor* Actor, FName NameBoneHit)
{
	Super::InitObject(Actor, NameBoneHit);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UMyShooter_StateEffect_ExecuteTimer::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UMyShooter_StateEffect_ExecuteTimer::Execute, RateTime, true);

	if (ParticleEffect)
	{
		FName NameBoneToAttached = NameBoneHit;
		FVector Loc = FVector(0);
		USceneComponent* myMesh = Cast<USceneComponent>(myActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (myMesh)
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myMesh, NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);

		}
		else
		{
			ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);

		}
	}
	return true;
}

void UMyShooter_StateEffect_ExecuteTimer::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	Super::DestroyObject();
}

void UMyShooter_StateEffect_ExecuteTimer::Execute()
{
	if (myActor)
	{
		UMyShooterHealthComponent* myHealthComp = Cast<UMyShooterHealthComponent>(myActor->GetComponentByClass(UMyShooterHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			myHealthComp->ChangeCurrentHealt(Power);
		}
	}
}






bool UMyShooter_StateEffect_ExecuteTimerStatsIncomeDamage::InitObject(AActor* Actor, FName NameBoneHit)
{
	Super::InitObject(Actor, NameBoneHit);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UMyShooter_StateEffect_ExecuteTimerStatsIncomeDamage::DestroyObject, Timer, false);
	
	if (ParticleEffect)
	{
		FName NameBoneToAttached = NameBoneHit;
		FVector Loc = FVector(0);

		ParticleEmitter = UGameplayStatics::SpawnEmitterAttached(ParticleEffect, myActor->GetRootComponent(), NameBoneToAttached, Loc, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	if (myActor)
	{
		UMyShooterHealthComponent* myHealthComp = Cast<UMyShooterHealthComponent>(myActor->GetComponentByClass(UMyShooterHealthComponent::StaticClass()));
		if (myHealthComp)
		{
			tempmark = myHealthComp->CoefDamage;
			myHealthComp->CoefDamage = myHealthComp->CoefDamage * CoefOfDamage;
		}
	}
	return true;
}


void UMyShooter_StateEffect_ExecuteTimerStatsIncomeDamage::DestroyObject()
{
	ParticleEmitter->DestroyComponent();
	ParticleEmitter = nullptr;
	UMyShooterHealthComponent* myHealthComp = Cast<UMyShooterHealthComponent>(myActor->GetComponentByClass(UMyShooterHealthComponent::StaticClass()));
	myHealthComp->CoefDamage = tempmark;
	tempmark = 0;
	Super::DestroyObject();
}


bool UMyShooter_StateEffect_ExecuteOnceRadial::InitObject(AActor* Actor, FName NameBoneHit)
{
	//Super::DestroyObject();
	Super::InitObject(Actor, NameBoneHit);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_EffectTimer, this, &UMyShooter_StateEffect_ExecuteOnceRadial::DestroyObject, Timer, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExecuteTimer, this, &UMyShooter_StateEffect_ExecuteOnceRadial::Execute, RateTime, true);

	if (NiagaraEffect)
	{
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, HitPoint, FRotator::ZeroRotator);
	
		float Scale = MinDamageRadius / 25.0f;
		NiagaraComponent->SetWorldScale3D(FVector(Scale));
	}

	return true;
}

void UMyShooter_StateEffect_ExecuteOnceRadial::DestroyObject()
{
	NiagaraComponent->DestroyComponent();
	NiagaraComponent = nullptr;
	Super::DestroyObject();
}

void UMyShooter_StateEffect_ExecuteOnceRadial::Execute()
{
	if (myActor)
	{
		if (SelfDamage && PawnDamage)
		{
			TArray<AActor*>	IgnoreActor;
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
				MaxDamage,
				MinDamage,
				HitPoint,
				MaxDamageRadius,
				MinDamageRadius,
				5,
				NULL, IgnoreActor, NULL, nullptr);
		}
		else if(!SelfDamage && PawnDamage)
		{
			TArray<AActor*>	IgnoreActor;
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
				MaxDamage,
				MinDamage,
				HitPoint,
				MaxDamageRadius,
				MinDamageRadius,
				5,
				NULL, IgnoreActor, myActor, nullptr);
		}
		/*else if (!PawnDamage)
		{
			TArray<AActor*>	IgnoreActor;
			UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(),
				MaxDamage,
				MinDamage,
				myActor->GetActorLocation(),
				MaxDamageRadius,
				MinDamageRadius,
				5,
				NULL, IgnoreActor, CHARACTER , nullptr);
		}*/
	}
}