// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponBase.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FShotIstigate, FVector, StartShotPoint, FVector, EndShotPoint, bool, bIsHitSomething, FHitResult, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadingStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadingComplete);
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
	int32 CurrentBulletCount;

	UFUNCTION(BlueprintCallable)
	FWeaponData GetWeaponData() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentBulletCount() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentMagazine() const;

	UFUNCTION(BlueprintCallable)
	EWeaponEvent InstigateWeaponShot(const bool bStopShooting);

	void ShootingFunction();

	void ReloadWeapon();

	void ReloadingFunction();

	//UPROPERTY(BlueprintAssignable)
	//FShotIstigate ShotInstigated;

	FTimerHandle ReloadingTimer;
	FTimerHandle ShootingTimer;

	UPROPERTY(BlueprintAssignable)
	FReloadingStart OnWeaponReloadStarted;

	UPROPERTY(BlueprintAssignable)
	FReloadingComplete OnWeaponReloadCompleted;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) //Called after weapon shot has been instigated
	void OnWeaponShotInstigated(FVector InStartShotPoint, FVector InEndShotPoint, bool bIsInHit, FHitResult InHitResult);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent) //Called before weapon shot has been instigated to get the start shoting point
	FTransform GetWeaponStartShotPoint() const;

	UFUNCTION(BlueprintCallable)
	int32 InitializeWeapon();
	
};
