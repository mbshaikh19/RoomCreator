// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorAreaManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSeedModify, FRandomStream&,newStream);

UCLASS()
class AUTOROOMCREATOR_API AFloorAreaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	static AFloorAreaManager* floorAreaManager;

	UPROPERTY()
	FSeedModify seedModify;

	static FRandomStream randomStream;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AFloorAreaManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SeedModified(FRandomStream& newStream);
};
