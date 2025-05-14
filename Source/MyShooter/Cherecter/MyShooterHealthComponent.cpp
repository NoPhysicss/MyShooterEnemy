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

	if (bIsHaveShield)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CoolDownShieldTimer, this, &UMyShooterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecover, false);
	}

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

float UMyShooterHealthComponent::GetCurrentShield()
{
	return Shield;
}

float UMyShooterHealthComponent::GetMaxShield()
{
	return MaxShield;
}

void UMyShooterHealthComponent::SetCurrentHealt(float NewHealth)
{
	CurrentHealth = NewHealth;
}

void UMyShooterHealthComponent::SetCurrentShield(float NewShield)
{
	Shield = NewShield;
}

void UMyShooterHealthComponent::ChangeCurrentHealt(float ChangeValue)
{
	if (!bIsHaveShield)
	{
		if (ChangeValue < 0.0f)
		{
			ChangeValue *= CoefDamage;
		}

		CurrentHealth += ChangeValue;
	}
	else
	{
		

		if (Shield > 0.0f && ChangeValue < 0.0f)
		{
			if (Shield < (ChangeValue * -1))
			{
				CurrentDamage = ChangeValue;
				CurrentDamage = ChangeValue + Shield;
				ChangeCurrentShield(ChangeValue);
				CurrentDamage *= CoefDamage;
				CurrentHealth += CurrentDamage;
			}
			else
			{
				ChangeCurrentShield(ChangeValue);
			}

			if (ChangeValue > 0.0f)
			{
				CurrentHealth += ChangeValue;
			}
		}
		else if (Shield <= 0.0f && ChangeValue < 0.0f)
		{
			ChangeValue *= CoefDamage;
			CurrentHealth += ChangeValue;
		}
		else if (ChangeValue > 0.0f)
		{
			CurrentHealth += ChangeValue;
		}
	}


	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}

	else if(CurrentHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
		GetOwner()->SetCanBeDamaged(false);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_CoolDownShieldTimer);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoverRateTimer);
		OnDead.Broadcast();
	}

	OnHealthChange.Broadcast(CurrentHealth, ChangeValue);
}

void UMyShooterHealthComponent::ChangeCurrentShield(float ChangeValue)
{

	Shield += ChangeValue;
	OnShieldChange.Broadcast(Shield, ChangeValue);
	if (Shield > MaxShield)
	{
		Shield = MaxShield;
	}
	else if (Shield <= 0.0f)
	{
		Shield = 0.0f;
	}

	if (GetWorld() && ChangeValue < 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoverRateTimer);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_CoolDownShieldTimer, this, &UMyShooterHealthComponent::CoolDownShieldEnd, CoolDownShieldRecover, false);
	}
}

void UMyShooterHealthComponent::CoolDownShieldEnd()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShieldRecoverRateTimer, this, &UMyShooterHealthComponent::RecoveryShield, ShieldRecoverRate, true);
	}
}

void UMyShooterHealthComponent::RecoveryShield()
{
	Shield += ShieldRecoverValue;
	if (Shield > MaxShield)
	{
		Shield = MaxShield;

		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ShieldRecoverRateTimer);
		}
	}
	OnShieldChange.Broadcast(Shield, ShieldRecoverValue);
}