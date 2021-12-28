// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	InventorySize = 0;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::SetInventorySize(const int32 ToSize)
{
	InventorySize = ToSize;
	Inventory.Init(FItemInventoryData(), ToSize);
}

bool UInventoryComponent::AddItemToInventory(const AItemBase* InItem)
{
	UClass* ItemClass = InItem->GetClass();
	
	for(int i = 0; i < Inventory.Num(); i++)
	{
		FItemInventoryData CurrentItem = Inventory[i];
		
		if(CurrentItem.ItemClass == nullptr)
		{
			const FItemData ItemInfo = InItem->ItemData;
			
			Inventory[i].ItemName = ItemInfo.ItemName;
			Inventory[i].ItemTexture = ItemInfo.ItemTexture;
			Inventory[i].ItemType = ItemInfo.ItemType;
			Inventory[i].ItemClass = ItemClass;

			OnItemAdded.Broadcast(ItemInfo, i);
			return true;
		}
	}
	return false;
}

void UInventoryComponent::RemoveItemFromInventory(const int32 OnIndex)
{
	if(Inventory.IsValidIndex(OnIndex))
	{
		Inventory[OnIndex] = FItemInventoryData();
		OnItemBeenRemoved(OnIndex);
		OnItemRemoved.Broadcast(OnIndex);
	}
}

TArray<FItemInventoryData> UInventoryComponent::GetInventory() const
{
	return Inventory;
}

int32 UInventoryComponent::GetInventorySize() const
{
	return InventorySize;
}
