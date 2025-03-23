// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyShooterHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, Damage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

USTRUCT()
struct FStatusParam
{
	GENERATED_BODY()

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTER_API UMyShooterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyShooterHealthComponent();

	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category= "Health")
	FOnHealthChange OnHealthChange;
	UPROPERTY(BlueprintAssignable, EditAnywhere, BlueprintReadWrite, Category = "Health")
	FOnDead OnDead;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	 
	UPROPERTY(EditAnywhere)
	float MaxHealth = 150.0f;

	UPROPERTY(EditAnywhere)
	float CurrentHealth = 100.0f;



public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CoefDamage = 1.0f;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetCurrentHealt();
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealt();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetCurrentHealt(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void ChangeCurrentHealt(float ChangeValue);



};
