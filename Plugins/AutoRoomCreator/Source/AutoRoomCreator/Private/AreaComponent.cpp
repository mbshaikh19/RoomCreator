// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaComponent.h"
#include "FloorAreaManager.h"
#include "PlaceableActor.h"

UAreaComponent::UAreaComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetVisibility(true);
	SetHiddenInGame(false);
}

void UAreaComponent::SetRandomSeed(FRandomStream& newStream)
{
	UE_LOG(LogTemp, Warning, TEXT("oooo Seed modified () UAreaComponent"));
}

void UAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	AFloorAreaManager::floorAreaManager->seedModify.AddDynamic(this, &UAreaComponent::SetRandomSeed);
	UE_LOG(LogTemp, Warning, TEXT("oooo BeginPlay () UAreaComponent"));
}

void UAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAreaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
