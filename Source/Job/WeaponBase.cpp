// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoFire = false;
	TotalAmmo = INDEX_NONE;
	ShotDistance = 0;
	CurrentMagazine = INDEX_NONE;
	AmmoToReload = 0;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FWeaponData AWeaponBase::GetWeaponData() const
{
	return WeaponData;
}

int32 AWeaponBase::GetTotalAmmo() const
{
	return TotalAmmo;
}

int32 AWeaponBase::GetCurrentMagazine() const
{
	return CurrentMagazine;
}

bool AWeaponBase::UseWeapon()
{
	if(AbleToUseWeapon())
	{
		//Magazine
		CurrentMagazine--;
		bIsUsingWeapon = true;
		
		//Timer
		GetWorldTimerManager().SetTimer(CooldownTimer, this, &AWeaponBase::StopRateDelay, WeaponData.ShootingRate, false);
		OnWeaponUse.Broadcast(this);

		//Trace
		FTransform StartShotPoint = GetWeaponStartShotPoint();
		FHitResult Hit;
		FVector Start = StartShotPoint.GetLocation();
		FVector End = Start + (StartShotPoint.GetRotation().Vector() * ShotDistance);
		FCollisionQueryParams TraceParams;
		bool bHitInstigated = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		//Particle
		if(bHitInstigated)
			UGameplayStatics::SpawnEmitterAtLocation(this, WeaponParticle, Hit.Location);
		else
			UGameplayStatics::SpawnEmitterAtLocation(this, WeaponParticle, End);

		//UKismetSystemLibrary::PrintString(this, "Shoot!", true, true, FLinearColor::Red, 10.0f);
		
		//Reload
		if(CurrentMagazine <= 0 && TotalAmmo > 0)
		{
			ReloadWeapon();
		}
		return true;
	}
	return false;
}

void AWeaponBase::ToggleWeaponUse(const bool bUse)
{
	//UKismetSystemLibrary::PrintString(this, "Use!", true, true, FLinearColor::Red, 10.0f);
	if(bUse)
	{
		if(AbleToUseWeapon())
		{
			UseWeapon();
			//UKismetSystemLibrary::PrintString(this, "Use!", true, true, FLinearColor::Red, 10.0f);
		}
	}
	else
	{
		if(bIsUsingWeapon)
		{
			StopUseWeapon();
		}
	}
}

void AWeaponBase::StopRateDelay()
{
	GetWorldTimerManager().ClearTimer(CooldownTimer);
	if(AutoFire)
	{
		UseWeapon();
	}
}

void AWeaponBase::StopUseWeapon()
{
	GetWorldTimerManager().ClearTimer(CooldownTimer);
	//UKismetSystemLibrary::PrintString(this, "StopShooting!", true, true, FLinearColor::Green, 10.0f);
	bIsUsingWeapon = false;
	OnStopWeaponUse.Broadcast(this);
}

bool AWeaponBase::AbleToUseWeapon() const
{
	return CurrentMagazine > 0 && !CooldownTimer.IsValid();
}

void AWeaponBase::ReloadWeapon()
{
	//UKismetSystemLibrary::PrintString(this, "ReloadStart!", true, true, FLinearColor::Green, 10.0f);
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::FinishReload, WeaponData.TimeToReload, false);
	AmmoToReload = FMath::Min(GetClass()->GetDefaultObject<AWeaponBase>()->WeaponData.MagazineSize - CurrentMagazine, TotalAmmo);
	OnReloadStart.Broadcast(this);
}

void AWeaponBase::FinishReload()
{
	CurrentMagazine = CurrentMagazine + AmmoToReload;
	TotalAmmo = TotalAmmo - AmmoToReload;
	
	if(bIsUsingWeapon)
		UseWeapon();
	OnReloadFinish.Broadcast(this);
	GetWorldTimerManager().ClearTimer(ReloadTimer);
}

int32 AWeaponBase::InitializeWeapon()
{
	if(TotalAmmo > WeaponData.MagazineSize)
	{
		CurrentMagazine = WeaponData.MagazineSize;
	}
	else
	{
		CurrentMagazine = TotalAmmo;
	}

	return CurrentMagazine;
}

FTimerHandle AWeaponBase::GetReloadTimer() const
{
	return ReloadTimer;
}


