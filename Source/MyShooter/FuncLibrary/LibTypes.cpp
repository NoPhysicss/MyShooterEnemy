// Fill out your copyright notice in the Description page of Project Settings.

#include "LibTypes.h"
#include "MyShooter/Interface/MyShooter_IGameActor.h"
#include "MyShooter/MyShooter.h"

void ULibTypes::AddEffectBySurfaceType(AActor* TakeEffectActor,FName NameBoneHit,TSubclassOf<UMyShooter_StateEffect> AddEffectClass, EPhysicalSurface SurfaceType, FVector HitLocation)
{
	if (SurfaceType != EPhysicalSurface::SurfaceType_Default && TakeEffectActor && AddEffectClass)
	{
		UMyShooter_StateEffect* myEffect = Cast<UMyShooter_StateEffect>(AddEffectClass->GetDefaultObject());
		if (myEffect)
		{
			bool bIsHavePossibleSurface = false;
			int8 i = 0;
			while (i < myEffect->PossibleInteractSurface.Num() && !bIsHavePossibleSurface)
			{
				if (myEffect->PossibleInteractSurface[i] == SurfaceType)
				{
					bIsHavePossibleSurface = true;
					bool bIsCanAddEffect = false;
					if (!myEffect->bIsStacable)
					{
						int8 y = 0;
						TArray<UMyShooter_StateEffect*> CurrentEffects;
						IMyShooter_IGameActor* myInterface = Cast<IMyShooter_IGameActor>(TakeEffectActor);
						if (myInterface)
						{
							CurrentEffects = myInterface->GetAllCurrentEffects();
						}
						if (CurrentEffects.Num() > 0)
						{
							while (y < CurrentEffects.Num() && bIsCanAddEffect)
							{
								if (CurrentEffects[y]->GetClass() != AddEffectClass)
								{
									bIsCanAddEffect = true;
								}
								y++;
							}
						}
						else
						{
							bIsCanAddEffect = true;
						}
					}
					else
					{
							bIsCanAddEffect = false;
					}
					if (bIsCanAddEffect)
					{

						
						UMyShooter_StateEffect* NewEffect = NewObject<UMyShooter_StateEffect>(TakeEffectActor, AddEffectClass);
						if (NewEffect)
						{
							NewEffect->LocationSave(HitLocation);
							NewEffect->InitObject(TakeEffectActor, NameBoneHit);	
						}

					}
				}
				i++;
			}
		}
		
	}
}
 