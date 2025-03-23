// Fill out your copyright notice in the Description page of Project Settings.


#include "MyShooter/Cherecter/MyShooterHealthComponent.h"

// Sets default values for this component's properties
UMyShooterHealthComponent::UMyShooterHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyShooterHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyShooterHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UMyShooterHealthComponent::GetCurrentHealt()
{
	return CurrentHealth;
}

float UMyShooterHealthComponent::GetMaxHealt()
{
	return MaxHealth;
}

void UMyShooterHealthComponent::SetCurrentHealt(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void UMyShooterHealthComponent::ChangeCurrentHealt(float ChangeValue)
{
	if (ChangeValue < 0.0f)
	{
		ChangeValue *= CoefDamage;
	}
	
	CurrentHealth += ChangeValue;

	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
	//	OnDead.Broadcast(); 
	}

	OnHealthChange.Broadcast(CurrentHealth, ChangeValue);
	if (CurrentHealth < 0.0f)
	{
		OnDead.Broadcast();
	}
	
}


