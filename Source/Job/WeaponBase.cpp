// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	return CurrentBulletCount;
}

int32 AWeaponBase::GetCurrentMagazine() const
{
	return CurrentMagazine;
}

EWeaponEvent AWeaponBase::InstigateWeaponShot(const bool bStopShooting)
{
	if(CurrentBulletCount > 0)
	{
		if(CurrentMagazine > 0)
		{
			if(!bStopShooting)
			{
				//ShootingTimer.Invalidate();
				GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeaponBase::ShootingFunction, WeaponData.ShootingFrequency, true);
				UKismetSystemLibrary::PrintString(this, "Started");
				ShootingFunction();
				return EWeaponEvent::Shooting;
			}
			ShootingTimer.Invalidate();
			UKismetSystemLibrary::PrintString(this, "Ended");
			return EWeaponEvent::StopShooting;
		}
		else
		{
			ReloadWeapon();
			return EWeaponEvent::Reload;
		}
	}
	return EWeaponEvent::Empty;
}

void AWeaponBase::ShootingFunction()
{
	if(CurrentMagazine > 0)
	{
		FTransform StartShotPoint = GetWeaponStartShotPoint();
		FHitResult Hit;

		FVector Start = StartShotPoint.GetLocation();
		FVector End = Start + (StartShotPoint.GetRotation().Vector() * ShotDistance);

		FCollisionQueryParams TraceParams;
		bool bHitInstigated = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

		// if(bHitInstigated)
		// {
		// 	if(Hit.bBlockingHit)
		// 	{
		// 		//UKismetSystemLibrary::DrawDebugSphere(this, End, 5.0f, 12, FLinearColor::White, 5.0f, 2.0f);
		// 		//UKismetSystemLibrary::PrintString(this, End.ToString());
		// 	}
		// }
		UKismetSystemLibrary::PrintString(this, "Shot");
		CurrentBulletCount--;
		CurrentMagazine--;
		
		OnWeaponShotInstigated(Start, End, bHitInstigated, Hit);
		//ShotInstigated.Broadcast(Start, End, bHitInstigated, Hit);
	}
	else
	{
		ReloadWeapon();
	}
}

void AWeaponBase::ReloadWeapon()
{
	// FTimerHandle ReloadingTimer;
	GetWorldTimerManager().SetTimer(ReloadingTimer, this, &AWeaponBase::ReloadingFunction, WeaponData.TimeToReload, false);
	ShootingTimer.Invalidate();
	OnWeaponReloadStarted.Broadcast();
	UKismetSystemLibrary::PrintString(this, "Reload");
}

void AWeaponBase::ReloadingFunction()
{
	UKismetSystemLibrary::PrintString(this, "Reloaded");
	CurrentMagazine = WeaponData.MagazineSize;
	ReloadingTimer.Invalidate();
	OnWeaponReloadCompleted.Broadcast();
}

int32 AWeaponBase::InitializeWeapon()
{
	if(WeaponData.MagazineSize <= CurrentBulletCount)
	{
		CurrentMagazine = WeaponData.MagazineSize;
		return CurrentMagazine;
	}
	CurrentMagazine = CurrentBulletCount;
	return CurrentMagazine;
}
