// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyShooter/Cherecter/MyShooterHealthComponent.h"
#include "MyShooterCharHealthComponent.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, Shield, float, Damage);


UCLASS()
class MYSHOOTER_API UMyShooterCharHealthComponent : public UMyShooterHealthComponent
{
	GENERATED_BODY()

public:

	FTimerHandle TimerHandle_CoolDownShieldTimer;
	FTimerHandle TimerHandle_ShieldRecoverRateTimer;

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnShieldChange OnShieldChange;

protected:

	void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float Shield = 100.0f;

	UPROPERTY(EditAnywhere)
	float MaxShield = 150.0f;

	UPROPERTY(EditAnywhere)
	float ShieldRecoverValue = 1.0f;

	UPROPERTY(EditAnywhere)
	float ShieldRecoverRate = 0.3f;

	UPROPERTY(EditAnywhere)
	float CoolDownShieldRecover = 2.0f;

public:

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentShield();

	void ChangeCurrentShield(float ChangeValue);

	void ChangeCurrentHealt(float ChangeValue) override;

	void CoolDownShieldEnd();

	void RecoveryShield();
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxShield();
};
