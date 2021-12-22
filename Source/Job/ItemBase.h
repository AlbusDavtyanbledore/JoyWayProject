// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructDeclarations.h"
#include "ItemBase.generated.h"

UCLASS(BlueprintType)
class JOB_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|ItemData", meta = (ExposeOnspawn = true))
	FItemData ItemData;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings|Mesh", meta = (ExposeOnSpawn = true))
	//bool bShouldSimulatePhysics;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UStaticMeshComponent* GetItemMeshComponent() const;
	
};
