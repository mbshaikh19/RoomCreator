// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AreaComponent.generated.h"

/**
 * 
 */
class APlaceableActor;
class AFloorAreaManager;

UCLASS()
class AUTOROOMCREATOR_API UAreaComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	TArray<TSubclassOf<APlaceableActor>> placeableObjectsList;

	TArray<APlaceableActor*> spawnedPlaceableObjects;
	
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UAreaComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void SetRandomSeed(FRandomStream &newStream);

	void PlaceObjectsInGrid();

	void PlaceObjectsByBFD();

	void AssignDynamicDelegate(AFloorAreaManager* floorAreaManager);

	void CleanPreviousResult();

	void shuffleList(FRandomStream& newStream);
};
