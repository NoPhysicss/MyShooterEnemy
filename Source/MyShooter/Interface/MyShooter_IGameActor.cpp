// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShooter_IGameActor.h"

// Add default functionality here for any IMyShooter_IGameActor functions that are not pure virtual.

EPhysicalSurface IMyShooter_IGameActor::GetSurfaceType()
{
	return EPhysicalSurface::SurfaceType_Default;
}

TArray<UMyShooter_StateEffect*> IMyShooter_IGameActor::GetAllCurrentEffects()
{
	return AllCurrentEffects;
}

void IMyShooter_IGameActor::RemoveEffect(UMyShooter_StateEffect* RemovedEffect)
{
}

void IMyShooter_IGameActor::AddEffect(UMyShooter_StateEffect* NewEffect)
{
}
