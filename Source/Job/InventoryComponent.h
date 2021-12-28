// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructDeclarations.h"
#include "ItemBase.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemAdded, FItemData, ItemData, int32, Index);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemRemoved, int32, OnIndex);

USTRUCT(BlueprintType)
struct FItemInventoryData : public FItemData
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItemBase> ItemClass;
	
	FItemInventoryData()
	{
		ItemClass = nullptr;
	}

	FItemInventoryData(const FName InItemName, UTexture2D* InItemTexture, const EItemType InItemType,  const TSubclassOf<class AItemBase> InItemClass)
	{
		ItemName = InItemName;
		ItemTexture = InItemTexture;
		ItemType = InItemType;
		ItemClass = InItemClass;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class JOB_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TArray<FItemInventoryData> Inventory;

	int32 InventorySize;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetInventorySize(const int32 ToSize);

	UFUNCTION(BlueprintCallable)
	bool AddItemToInventory(const AItemBase* InItem);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromInventory(const int32 OnIndex);

	UPROPERTY(BlueprintAssignable)
	FItemAdded OnItemAdded;

	UPROPERTY(BlueprintAssignable)
	FItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemBeenRemoved(int32 OfIndex);

	UFUNCTION(BlueprintCallable)
	TArray<FItemInventoryData> GetInventory() const;

	UFUNCTION(BlueprintCallable)
	int32 GetInventorySize() const;
};
