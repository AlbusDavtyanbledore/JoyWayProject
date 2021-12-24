// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "SlateCore/Public/Brushes/SlateImageBrush.h"
#include "StructDeclarations.generated.h"

class AItemBase;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Weapon,
	Grenade,
	Foutniture
};

UENUM(BlueprintType)
enum class EWeaponEvent : uint8
{
	None,
	Shooting,
	StopShooting,
	Reload,
	Empty
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const UTexture2D* ItemTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType;
	
	FItemData()
	{
		ItemName = "None";
		ItemTexture = nullptr;
		ItemType = EItemType::None;
	}

	FItemData(const FName InItemName, UTexture2D* InItemTexture, const EItemType InItemType)
	{
		ItemName = InItemName;
		ItemTexture = InItemTexture;
		ItemType = InItemType;
	}
};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagazineSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootingFrequency;
	
	FWeaponData()
	{
		MagazineSize = 0;
		DamageAmount = 0.0f;
		TimeToReload = 0.f;
		ShootingFrequency = 0.0f;
		
	}

	FWeaponData(const int32 InMagazineSize, const float InDamageAmount, const float InTimeToReload, const float InShootingFrequency)
	{
		MagazineSize = InMagazineSize;
		DamageAmount = InDamageAmount;
		TimeToReload = InTimeToReload;
		ShootingFrequency = InShootingFrequency;
	}
};

