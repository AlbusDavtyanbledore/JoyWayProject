// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "SlateCore/Public/Brushes/SlateImageBrush.h"
#include "StructDeclarations.generated.h"

class AItemBase;

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemTexture;
	
	FItemData()
	{
		ItemName = "None";
		ItemClass = nullptr;
		ItemTexture = nullptr;
	}
};

