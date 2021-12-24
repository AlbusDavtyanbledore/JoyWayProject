// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponBase.generated.h"

class AWeaponBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponUse, AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopWeaponUse, AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadStart, AWeaponBase*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadFinish, AWeaponBase*, Weapon);

/**
 * 
 */
UCLASS(BlueprintType)
class JOB_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

	public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|WeaponData", meta = (ExposeOnspawn = true))
	FWeaponData WeaponData;

	UPROPERTY(BlueprintReadOnly, Category = "Settings|WeaponData")
	int32 CurrentMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|WeaponData")
	float ShotDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|WeaponData")
	int32 TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|WeaponData")
	bool AutoFire;
	
	bool bFireIsActive;

	UFUNCTION(BlueprintCallable)
	FWeaponData GetWeaponData() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBulletCount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentMagazine() const;

	bool UseWeapon();

	UFUNCTION(BlueprintCallable)
	void ToggleWeaponUse(const bool bUse);

	bool bIsReloading;
	bool bIsUsingWeapon;

	int32 AmmoToReload;

	void StopRateDelay();

	void StopUseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|WeaponData")
	UParticleSystem* WeaponParticle;

	bool AbleToUseWeapon() const;

	FTimerHandle CooldownTimer;
	FTimerHandle ReloadTimer;

	UPROPERTY(BlueprintAssignable)
	FWeaponUse OnWeaponUse;

	UPROPERTY(BlueprintAssignable)
	FStopWeaponUse OnStopWeaponUse;

	UPROPERTY(BlueprintAssignable)
	FReloadStart OnReloadStart;

	UPROPERTY(BlueprintAssignable)
	FReloadFinish OnReloadFinish;

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();
	
	void FinishReload();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) //Called before weapon shot has been instigated to get the start shoting point
	FTransform GetWeaponStartShotPoint() const;

	UFUNCTION(BlueprintCallable)
	int32 InitializeWeapon();
	
};
