// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "MyShooter/FuncLibrary/LibTypes.h"
#include "MyInventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSwitchWeapon, FName, WeaponIdName, FAdditionalWeaponInfo, WeaponAdditionalInfo, int32, NewCurrentIndexWeapon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChange, EWeaponType, TypeAmmo, int32, Cout);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponAdditionalInfoChange, int32, IndexSlot, FAdditionalWeaponInfo, AdditionalInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoEmpty, EWeaponType, WeaponType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponAmmoAviable, EWeaponType, WeaponType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateWeaponSlots, int32, IndexSlotChange, FWeaponSlot, NewInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponNotHaveRound, int32, IndexSlotWeapon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponHaveRound, int32, IndexSlotWeapon);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYSHOOTER_API UMyInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMyInventoryComponent();


	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSwitchWeapon OnSwitchWeapon;
	//Event on change ammo in slots by weaponType
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnAmmoChange OnAmmoChange;
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAdditionalInfoChange OnWeaponAdditionalInfoChange;
	//Event Ammo slots after change still empty rounds
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAmmoEmpty OnWeaponAmmoEmpty;
	//Event Ammo slots after chage have rounds
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponAmmoAviable OnWeaponAmmoAviable;
	//Event weapon was change by slotIndex
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnUpdateWeaponSlots OnUpdateWeaponSlots;

	//Event current weapon not have additional_Rounds 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponNotHaveRound OnWeaponNotHaveRound;
	//Event current weapon have additional_Rounds 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponHaveRound OnWeaponHaveRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FWeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<FAmmoSlot> AmmoSlots;

	int32 MaxSlotsWeapon = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	bool SwitchWeaponToIndexByNextPreviosIndex(int32 ChangeToIndex, int32 OldIndex, FAdditionalWeaponInfo OldInfo, bool bIsForward);
	bool SwitchWeaponByIndex(int32 IndexWeaponToChange, int32 PreviosIndex, FAdditionalWeaponInfo PreviosWeaponInfo);

	void SetAdditionalInfoWeapon(int32 IndexWeapon, FAdditionalWeaponInfo NewInfo);

	FAdditionalWeaponInfo GetAdditionalInfoWeapon(int32 IndexWeapon);
	int32 GetWeaponIndexSlotByName(FName IdWeaponName);
	FName GetWeaponNameBySlotIndex(int32 indexSlot);
	bool GetWeaponTypeByIndexSlot(int32 IndexSlot, EWeaponType& WeaponType);
	bool GetWeaponTypeByNameWeapon(FName IdWeaponName, EWeaponType& WeaponType);

	UFUNCTION(BlueprintCallable)
	void AmmoSlotChangeValue(EWeaponType TypeWeapon, int32 CoutChangeAmmo);
	bool CheckAmmoForWeapon(EWeaponType TypeWeapon, int32 &AviableAmmoForWeapon);

	//Interface PickUp Actors
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeAmmo(EWeaponType AmmoType);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool CheckCanTakeWeapon(int32& FreeSlot);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool SwitchWeaponToInventory(FWeaponSlot NewWeapon, int32 IndexSlot, int32 CurrentIndexWeaponChar, FDropItem& DropItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool TryGetWeaponToInventory(FWeaponSlot NewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Interface")
	bool GetDropItemInfoFromInventory(int32 IndexSlot, FDropItem& DropItemInfo);

	
	bool alreadyhas = false;


	UFUNCTION(BlueprintCallable, Category = "Inv")
	void InitInventory(TArray<FWeaponSlot> NewWeaponSlotsInfo, TArray<FAmmoSlot> NewAmmoSlotsInfo);

};
