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
	bIsReloading = false;
	TotalAmmo = 0;
	ShotDistance = 0;
	CurrentMagazine = 0;
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

int32 AWeaponBase::GetCurrentBulletCount() const
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
		//Timer
		CurrentMagazine--;
		bIsUsingWeapon = true;
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
		UGameplayStatics::SpawnEmitterAttached(WeaponParticle, GetItemMeshComponent(), "root",GetWeaponStartShotPoint().GetLocation(),
			FRotator(0,0,0), FVector(0.3,0.3,0.3), EAttachLocation::KeepWorldPosition, true, EPSCPoolMethod::None, true);

		//Reload
		if(CurrentMagazine <= 0 && TotalAmmo > 0)
		{
			ReloadWeapon();
		}
		return true;
	}
	bIsUsingWeapon = false;
	return false;
}

void AWeaponBase::ToggleWeaponUse(const bool bUse)
{
	if(bUse)
	{
		if(AbleToUseWeapon())
		{
			UseWeapon();
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
	bIsUsingWeapon = false;
	OnStopWeaponUse.Broadcast(this);
}

bool AWeaponBase::AbleToUseWeapon() const
{
	return CurrentMagazine > 0 && !CooldownTimer.IsValid();
}

void AWeaponBase::ReloadWeapon()
{
	if(!bIsReloading)
	{
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeaponBase::FinishReload, WeaponData.TimeToReload, false);
		AmmoToReload = FMath::Min(GetClass()->GetDefaultObject<AWeaponBase>()->GetCurrentMagazine() - CurrentMagazine, TotalAmmo);
		OnReloadStart.Broadcast(this);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(ReloadTimer);
		OnReloadFinish.Broadcast(this);
	}
}

void AWeaponBase::FinishReload()
{
	bIsReloading = false;
	ReloadWeapon();
	CurrentMagazine = CurrentMagazine + AmmoToReload;
	TotalAmmo = TotalAmmo - AmmoToReload;
}

int32 AWeaponBase::InitializeWeapon()
{
	if(WeaponData.MagazineSize <= TotalAmmo)
	{
		CurrentMagazine = WeaponData.MagazineSize;
		return CurrentMagazine;
	}
	CurrentMagazine = TotalAmmo;
	return CurrentMagazine;
}


