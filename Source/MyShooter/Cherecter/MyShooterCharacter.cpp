// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "MyShooter/MyShooterInstance.h"
#include "Math.h"
#include "MyShooter/ProjectileDefault.h"
#include "Engine/World.h"



AMyShooterCharacter::AMyShooterCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	bIsBloom = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCharacterMovement()->MaxWalkSpeed = MovementInfo.NormalSpeed;

	InventoryComponent = CreateDefaultSubobject<UMyInventoryComponent>(TEXT("InventoryComponent"));
	CharHealthComponent = CreateDefaultSubobject<UMyShooterCharHealthComponent>(TEXT("HealthComponente"));

	if (CharHealthComponent)
	{
		CharHealthComponent->OnDead.AddDynamic(this, &AMyShooterCharacter::CharDead);
	}
	if (InventoryComponent)
	{
		InventoryComponent->OnSwitchWeapon.AddDynamic(this, &AMyShooterCharacter::InitWeapon);
	}
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMyShooterCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	MovementTick(DeltaSeconds);

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}
}

void AMyShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursoreSize, FVector(0));
	}
	if (GetWorld())
	{
		AGameModeBase* GM = GetWorld()->GetAuthGameMode();
		if (GM)
		{
			UE_LOG(LogTemp, Warning, TEXT("GameMode at Start: %s"), *GM->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode is NULL in BeginPlay!"));
		}
	}
}

void AMyShooterCharacter::SetupPlayerInputComponent(UInputComponent* NewInputComponent)
{
	Super::SetupPlayerInputComponent(NewInputComponent);

	NewInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyShooterCharacter::MoveInputAxisX);
	NewInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyShooterCharacter::MoveInputAxisY);

	NewInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &AMyShooterCharacter::InputAttackPressed);
	NewInputComponent->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &AMyShooterCharacter::InputAttackReleased);
	NewInputComponent->BindAction(TEXT("ReloadEvent"), EInputEvent::IE_Released, this, &AMyShooterCharacter::TryReloadWeapon);

	NewInputComponent->BindAction(TEXT("SwitchNextWeapon"), EInputEvent::IE_Pressed, this, &AMyShooterCharacter::TrySwicthNextWeapon);
	NewInputComponent->BindAction(TEXT("SwitchPreviosWeapon"), EInputEvent::IE_Pressed, this, &AMyShooterCharacter::TrySwitchPreviosWeapon);

	NewInputComponent->BindAction(TEXT("AbilityAction"), EInputEvent::IE_Pressed, this, &AMyShooterCharacter::TryAbilityEnabled);

	TArray<FKey> HotKeys;
	HotKeys.Add(EKeys::One);
	HotKeys.Add(EKeys::Two);
	HotKeys.Add(EKeys::Three);
	HotKeys.Add(EKeys::Four);
	HotKeys.Add(EKeys::Five);
	HotKeys.Add(EKeys::Six);
	HotKeys.Add(EKeys::Seven);
	HotKeys.Add(EKeys::Eight);
	HotKeys.Add(EKeys::Nine);
	HotKeys.Add(EKeys::Zero);

	NewInputComponent->BindKey(HotKeys[1], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<1>);
	NewInputComponent->BindKey(HotKeys[2], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<2>);
	NewInputComponent->BindKey(HotKeys[3], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<3>);
	NewInputComponent->BindKey(HotKeys[4], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<4>);
	NewInputComponent->BindKey(HotKeys[5], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<5>);
	NewInputComponent->BindKey(HotKeys[6], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<6>);
	NewInputComponent->BindKey(HotKeys[7], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<7>);
	NewInputComponent->BindKey(HotKeys[8], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<8>);
	NewInputComponent->BindKey(HotKeys[9], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<9>);
	NewInputComponent->BindKey(HotKeys[0], IE_Pressed, this, &AMyShooterCharacter::TKeyPressed<0>);
	
}

void AMyShooterCharacter::MoveInputAxisY(float Value)
{
	AxisY = Value;
}

void AMyShooterCharacter::MoveInputAxisX(float Value)
{
	AxisX = Value;
}

void AMyShooterCharacter::InputAttackPressed()
{
	if (bIsAlive)
	{
		AttackCharEvent(true);
	}
}

void AMyShooterCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void AMyShooterCharacter::MovementTick(float DeltaTime)
{
	if (bIsAlive)
	{
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
		AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

		if (AxisX != 0.0f || AxisY != 0.0f)
		{
			NormalWalkEnable = true;
			ChangeMovementState();
		}
		else if (AxisX == 0.0f && AxisY == 0.0f)
		{
			NormalWalkEnable = false;
			ChangeMovementState();
		}

		FHitResult ResultHit;

		if (MovementState == EMovementState::Run_State)
		{
			FVector myRotationVector = FVector(AxisX, AxisY, 0.0f);
			FRotator myRotator = myRotationVector.ToOrientationRotator();
			SetActorRotation((FQuat(myRotator)));
		}
		else
		{
			APlayerController* MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (MyController)
			{

				//MyController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, false, ResultHit);
				MyController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
				SetActorRotation(FQuat(FRotator(0.0f, UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw, 0.0f)));
			}
			if (CurrentWeapon)
			{
				FVector Displacement = FVector(0);
				switch (MovementState)
				{
				case EMovementState::Aim_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					CurrentWeapon->ShouldReduceDispersion = true;
					break;
				case EMovementState::AimWalk_State:
					Displacement = FVector(0.0f, 0.0f, 160.0f);
					CurrentWeapon->ShouldReduceDispersion = true;
					break;
				case EMovementState::Normal_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->ShouldReduceDispersion = false;
					break;
				case EMovementState::Walk_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->ShouldReduceDispersion = false;
					break;
				case EMovementState::Stand_State:
					Displacement = FVector(0.0f, 0.0f, 120.0f);
					CurrentWeapon->ShouldReduceDispersion = false;
					break;
				case EMovementState::Run_State:
					break;
				default:
					break;
				}
				CurrentWeapon->ShootEndLocation = ResultHit.Location + Displacement;
			}
		}
	}
}

void AMyShooterCharacter::CharacterUpdate()
{
	float ResSpeed = 350;
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementInfo.AimSpeed;
		break;
	case EMovementState::Walk_State:
		ResSpeed = MovementInfo.WalkSpeed;
		break;
	case EMovementState::Run_State:
		ResSpeed = MovementInfo.RunSpeed;
		break;
	case EMovementState::AimWalk_State:
		ResSpeed = MovementInfo.AimWalkSpeed;
		break;
	case EMovementState::Normal_State:
		ResSpeed = MovementInfo.NormalSpeed;
		break;
	case EMovementState::Stand_State:
		ResSpeed = MovementInfo.StandSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void AMyShooterCharacter::ChangeMovementState()
{
	if (!WalkEnable && !RunEnable && !AimEnable && !NormalWalkEnable)
	{
		MovementState = EMovementState::Stand_State;
	}
	else if (!WalkEnable && !RunEnable && !AimEnable && NormalWalkEnable)
	{
		MovementState = EMovementState::Normal_State;
	}
	else 
	{
		if (RunEnable)
		{
			WalkEnable = false;
			AimEnable = false;
			MovementState = EMovementState::Run_State;
		}
		if (WalkEnable && !RunEnable && AimEnable)
		{
			MovementState = EMovementState::AimWalk_State;
		}
		else
		{
			if (WalkEnable && !RunEnable && !AimEnable)
			{
				MovementState = EMovementState::Walk_State;
			}
			else if(!WalkEnable && !RunEnable && AimEnable)
			{
				MovementState = EMovementState::Aim_State;
			}
		}
	}
	CharacterUpdate();

	AWeaponDefault* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}



void AMyShooterCharacter::RunMovementState()
{
	float ResSpeed = MovementInfo.RunSpeed;
	LookDirection = GetActorForwardVector();
	MoveDirection = FVector(AxisX, AxisY, 0.0f).GetSafeNormal();

	if (!MoveDirection.IsNearlyZero())
	{
		float DotProduct = FVector::DotProduct(MoveDirection, LookDirection);
		float Angle = FMath::Acos(DotProduct) * 180 / PI;


		SpeedModif = FMath::Clamp(1.0f - (Angle / 180), 0.0f, 1.0f);
	}
	ResSpeed *= SpeedModif;
	if (ResSpeed < MovementInfo.NormalSpeed)
	{
		ResSpeed = MovementInfo.NormalSpeed;
	}
	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;

	AWeaponDefault* myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->UpdateStateWeapon(MovementState);
	}
}

void AMyShooterCharacter::BCameraInputAxis(float Value)
{
	CameraAxis = Value;
	BoomCameraWeight();
}

void AMyShooterCharacter::BoomCameraWeight()
{
	float CurrentCameraWeight = CameraBoom->TargetArmLength;

	if (!bIsBloom)
	{
		if (CameraAxis < 0)
		{
			if ((CurrentCameraWeight + CameraWeightChange) < CameraMaxWeight)
			{
				CameraBoom->TargetArmLength = CurrentCameraWeight + CameraWeightChange;
			}
		}
		else if (CameraAxis > 0)
		{
			if ((CurrentCameraWeight - CameraWeightChange) > CameraMinWeight)
			{	
				CameraBoom->TargetArmLength = CurrentCameraWeight - CameraWeightChange;
			}
		}
	}

}

UDecalComponent* AMyShooterCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}

EMovementState AMyShooterCharacter::GetMovementState()
{
	return MovementState;
}

TArray<UMyShooter_StateEffect*> AMyShooterCharacter::GetCurrentEffectsOnChar()
{
	return Effects;
}

int32 AMyShooterCharacter::GetCurrentWeaponIndex()
{
	return CurrentIndexWeapon;
}

void AMyShooterCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefault* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		//ToDo Check melee or range
		myWeapon->SetWeaponStateFire(bIsFiring);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::AttackCharEvent - CurrentWeapon -NULL"));
}

AWeaponDefault* AMyShooterCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void AMyShooterCharacter::InitWeapon(FName IdWeaponName, FAdditionalWeaponInfo WeaponAdditionalInfo, int32 NewCurrentIndexWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	UMyShooterInstance* myGI = Cast<UMyShooterInstance>(GetGameInstance());
	FWeaponInfo MyWeaponInfo;
	if (myGI)
	{
		if (myGI->GetWeaponInfoByName(IdWeaponName, MyWeaponInfo))
		{
			if (MyWeaponInfo.WeaponClass)
			{
				FVector SpawnLocation = FVector(0);
				FRotator SpawnRotation = FRotator(0);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				AWeaponDefault* myWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(MyWeaponInfo.WeaponClass, &SpawnLocation, &SpawnRotation, SpawnParams));
				if (myWeapon)
				{
					FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
					myWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
					CurrentWeapon = myWeapon;

					myWeapon->WeaponSetting = MyWeaponInfo;

					//myWeapon->WeaponInfo.Round = MyWeaponInfo.MaxRound;

					myWeapon->ReloadTime = MyWeaponInfo.ReloadTime;
					myWeapon->UpdateStateWeapon(MovementState);

					myWeapon->AdditionalWeaponInfo = WeaponAdditionalInfo;

					CurrentIndexWeapon = NewCurrentIndexWeapon;

					myWeapon->OnWeaponReloadStart.AddDynamic(this, &AMyShooterCharacter::WeaponReloadStart);
					myWeapon->OnWeaponReloadEnd.AddDynamic(this, &AMyShooterCharacter::WeaponReloadEnd);
					myWeapon->OnWeaponFireStart.AddDynamic(this, &AMyShooterCharacter::WeaponFireStart);

					if (CurrentWeapon->GetWeaponRound() <= 0 && CurrentWeapon->CheckCanWeaponReload())
						CurrentWeapon->InitReload();

					//if (InventoryComponent)
					//	InventoryComponent->OnWeaponAmmoAviable.Broadcast(myWeapon->WeaponSetting.WeaponType);

				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::InitWeapon - Weapon not found in table -NULL"));
		}
	}


}

void AMyShooterCharacter::TryReloadWeapon()
{
	if (bIsAlive && CurrentWeapon && !CurrentWeapon->WeaponReloading)//fix reload
	{
		if (CurrentWeapon->GetWeaponRound() < CurrentWeapon->WeaponSetting.MaxRound && CurrentWeapon->CheckCanWeaponReload())
			CurrentWeapon->InitReload();
	}
}

void AMyShooterCharacter::WeaponReloadStart(UAnimMontage* Anim)
{
	WeaponReloadStart_BP(Anim);
}

void AMyShooterCharacter::WeaponReloadEnd(bool bIsSuccess, int32 AmmoTake)
{
	if (InventoryComponent && CurrentWeapon)
	{
		InventoryComponent->AmmoSlotChangeValue(CurrentWeapon->WeaponSetting.WeaponType, AmmoTake);
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	}
	WeaponReloadEnd_BP(bIsSuccess);
}

bool AMyShooterCharacter::TrySwitchWeaponToIndexByKeyInput(int32 ToIndex)
{
	bool bIsSuccess = false;
	if (CurrentWeapon && !CurrentWeapon->WeaponReloading && InventoryComponent->WeaponSlots.IsValidIndex(ToIndex))
	{
		if (CurrentIndexWeapon != ToIndex && InventoryComponent)
		{
			int32 OldIndex = CurrentIndexWeapon;
			FAdditionalWeaponInfo OldInfo;

			if (CurrentWeapon)
			{
				OldInfo = CurrentWeapon->AdditionalWeaponInfo;
				if (CurrentWeapon->WeaponReloading)
					CurrentWeapon->CancelReload();
			}

			bIsSuccess = InventoryComponent->SwitchWeaponByIndex(ToIndex, OldIndex, OldInfo);
		}
	}
	return bIsSuccess;
}

void AMyShooterCharacter::WeaponReloadEnd_BP_Implementation(bool bIsSuccess)
{
	//in BP
}

void AMyShooterCharacter::WeaponReloadStart_BP_Implementation(UAnimMontage* Anim)
{
	//in BP
}

void AMyShooterCharacter::WeaponFireStart(UAnimMontage* Anim)
{
	if (InventoryComponent && CurrentWeapon)
		InventoryComponent->SetAdditionalInfoWeapon(CurrentIndexWeapon, CurrentWeapon->AdditionalWeaponInfo);
	WeaponFireStart_BP(Anim);
}

void AMyShooterCharacter::WeaponFireStart_BP_Implementation(UAnimMontage* Anim)
{
	//in BP
}

void AMyShooterCharacter::TrySwicthNextWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		//We have more then one weapon go switch
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon && InventoryComponent->WeaponSlots.Num() > 1)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon->CancelReload();
		}
		if (!CurrentWeapon)
		{
			OldInfo = InventoryComponent->WeaponSlots[0].AdditionalInfo;

			InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(0, 0, OldInfo, true);

		}
		if (InventoryComponent)
		{
			if (InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(CurrentIndexWeapon + 1, OldIndex, OldInfo, true))
			{
			}
		}
	}
}

void AMyShooterCharacter::TrySwitchPreviosWeapon()
{
	if (InventoryComponent->WeaponSlots.Num() > 1)
	{
		//We have more then one weapon go switch
		int8 OldIndex = CurrentIndexWeapon;
		FAdditionalWeaponInfo OldInfo;
		if (CurrentWeapon && InventoryComponent->WeaponSlots.Num() > 1)
		{
			OldInfo = CurrentWeapon->AdditionalWeaponInfo;
			if (CurrentWeapon->WeaponReloading)
				CurrentWeapon->CancelReload();
		}
		if (!CurrentWeapon)
		{
			OldInfo = InventoryComponent->WeaponSlots[0].AdditionalInfo;

			InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(0, 0, OldInfo, true);

		}
		if (InventoryComponent)
		{
			//InventoryComponent->SetAdditionalInfoWeapon(OldIndex, GetCurrentWeapon()->AdditionalWeaponInfo);
			if (InventoryComponent->SwitchWeaponToIndexByNextPreviosIndex(CurrentIndexWeapon - 1, OldIndex, OldInfo, false))
			{
			}
		}
	}
}

float AMyShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage;

	//Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (bIsAlive)
	{
		CharHealthComponent->ChangeCurrentHealt(-DamageAmount);
	}
	 

	if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		AProjectileDefault* myProjectile = Cast<AProjectileDefault>(DamageCauser);
		if (myProjectile)
		{
			ULibTypes::AddEffectBySurfaceType(this, NAME_None, myProjectile->ProjectileSetting.Effect, GetSurfaceType(), GetOwner()->GetActorLocation());
			
		}
	}

	return ActualDamage;
}

void AMyShooterCharacter::CharDead()
{
	float AnimTime = 0.0f;
	int32 rnd = FMath::RandHelper(DeadsAnim.Num());
	if (DeadsAnim.IsValidIndex(rnd) && DeadsAnim[rnd] && GetMesh() && GetMesh()->GetAnimInstance())
	{
		AnimTime = DeadsAnim[rnd]->GetPlayLength();
		GetMesh()->GetAnimInstance()->Montage_Play(DeadsAnim[rnd]);
	}

	bIsAlive = false;
 
	if (GetController())
	{
		GetController()->UnPossess();
	}

	UnPossessed();

	GetWorldTimerManager().SetTimer(TimerHandle_RagDollTimer,this, &AMyShooterCharacter::EnableRagDoll, AnimTime - 0.4f, false);
	
	AttackCharEvent(false);

	CharDead_BP();
}

void AMyShooterCharacter::EnableRagDoll()
{
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetSimulatePhysics(true);
	}
}

EPhysicalSurface AMyShooterCharacter::GetSurfaceType()
{
	EPhysicalSurface Result = EPhysicalSurface::SurfaceType_Default;
	if (CharHealthComponent)
	{
		if (CharHealthComponent->GetCurrentShield() <= 0.0f)
		{
			if (GetMesh())
			{
				UMaterialInterface* myMaterial = GetMesh()->GetMaterial(0);
				if (myMaterial)
				{
					Result = myMaterial->GetPhysicalMaterial()->SurfaceType;
				}
			}
		}
	}
	return Result;
}

TArray<UMyShooter_StateEffect*> AMyShooterCharacter::GetAllCurrentEffects()
{
	return Effects;
}

void AMyShooterCharacter::RemoveEffect(UMyShooter_StateEffect* RemovedEffect)
{
	Effects.Remove(RemovedEffect);
}

void AMyShooterCharacter::AddEffect(UMyShooter_StateEffect* NewEffect)
{
	Effects.Add(NewEffect);
}


void AMyShooterCharacter::CharDead_BP_Implementation()
{
	//BP
}

void AMyShooterCharacter::TryAbilityEnabled()
{
	if (AbilityEffect)
	{
		UMyShooter_StateEffect* NewEffect = NewObject<UMyShooter_StateEffect>(this, AbilityEffect);
		if (NewEffect)
		{
			NewEffect->InitObject(this, NAME_None);
		}
	}
}

