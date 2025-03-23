// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyShooter/FuncLibrary/LibTypes.h"
#include "TimerManager.h"
#include "MyShooter/WeaponDefault.h"
#include "MyShooter/MyShooterInstance.h"
#include "MyShooter/Cherecter/MyInventoryComponent.h"
#include "MyShooter/Cherecter/MyShooterCharHealthComponent.h"
#include "MyShooter/Cherecter/MyShooterHealthComponent.h"
#include "MyShooter/Game/MyShooterPlayerController.h"
#include "MyShooter/MyShooter_StateEffect.h"
#include "Engine/DamageEvents.h"
#include "MyShooter/Interface/MyShooter_IGameActor.h"
#include "MyShooterCharacter.generated.h"


UCLASS(Blueprintable)
class AMyShooterCharacter : public ACharacter, public IMyShooter_IGameActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;


	//Inputs
	UFUNCTION()
	void MoveInputAxisY(float Value);
	UFUNCTION()
	void MoveInputAxisX(float Value);

	UFUNCTION(BlueprintCallable)
	void BCameraInputAxis(float Value);

	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	//Inventory Inputs
	void TrySwicthNextWeapon();
	void TrySwitchPreviosWeapon();

	//Ability Inputs
	void TryAbilityEnabled();

	template<int32 Id>
	void TKeyPressed()
	{
		TrySwitchWeaponToIndexByKeyInput(Id);
	}
	//Inputs End

	//Input Flags

	FVector LookDirection;
	FVector MoveDirection;
	float SpeedModif = 1.0f;

	float AxisX = 0.0f;
	float AxisY = 0.0f;

	bool bisReloding = false;

	UFUNCTION(BlueprintCallable)
	void BoomCameraWeight();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraWeightChange = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraMaxWeight = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraMinWeight = 200.0f;
	bool bIsBloom;
	float CameraAxis = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool RunEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool NormalWalkEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Normal_State;

	AWeaponDefault* CurrentWeapon = nullptr;

	UDecalComponent* CurrentCursor = nullptr;

	TArray<UMyShooter_StateEffect*> Effects;

	int32 CurrentIndexWeapon = 0;

	UFUNCTION()
	void CharDead();

	void EnableRagDoll();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	AMyShooterCharacter();

	FTimerHandle TimerHandle_RagDollTimer;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputMoveComponent) override;


	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UMyInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UMyShooterCharHealthComponent* CharHealthComponent;

	//Cursor material on decal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursoreSize = FVector(20.f, 40.f, 40.f);

	//Default move rule and state character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementSpeedInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	TArray<UAnimMontage*> DeadsAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UMyShooter_StateEffect> AbilityEffect;

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	// Tick Func
	UFUNCTION()
	void MovementTick(float DeltaTime);

	// Tick Func End


	//Funcs
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();
	UFUNCTION(BlueprintCallable)
	void RunMovementState();

	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);

	UFUNCTION(BlueprintCallable)
	void InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon);
	UFUNCTION(BlueprintCallable)
	void TryReloadWeapon();
	UFUNCTION()
	void WeaponFireStart(UAnimMontage* Anim);
	UFUNCTION(BlueprintCallable)
	void WeaponReloadStart(UAnimMontage* Anim);
	UFUNCTION()
	void WeaponReloadEnd(bool bIsSuccess, int32 AmmoSafe);

	bool TrySwitchWeaponToIndexByKeyInput(int32 ToIndex);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponFireStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadStart_BP(UAnimMontage* Anim);
	UFUNCTION(BlueprintNativeEvent)
	void WeaponReloadEnd_BP(bool bIsSuccess);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UDecalComponent* GetCursorToWorld();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EMovementState GetMovementState();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UMyShooter_StateEffect*> GetCurrentEffectsOnChar();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCurrentWeaponIndex();
	//Func End

	//Interface
	EPhysicalSurface GetSurfaceType() override;
	TArray<UMyShooter_StateEffect*> GetAllCurrentEffects() override;
	void RemoveEffect(UMyShooter_StateEffect* RemovedEffect) override;
	void AddEffect(UMyShooter_StateEffect* NewEffect) override;
	//Interface End

	UFUNCTION(BlueprintNativeEvent)
	void CharDead_BP();


};

