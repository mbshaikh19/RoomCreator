// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorAreaManager.h"

AFloorAreaManager* AFloorAreaManager::floorAreaManager;
// Sets default values
AFloorAreaManager::AFloorAreaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFloorAreaManager::BeginPlay()
{
	Super::BeginPlay();
	floorAreaManager = this;

	seedModify.AddDynamic(this, &AFloorAreaManager::SeedModified);
}

// Called every frame
void AFloorAreaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorAreaManager::SeedModified(FRandomStream& newStream)
{
	//randomStream.Initialize(inSeed);
	UE_LOG(LogTemp, Warning, TEXT("oooo Seed modified () FloorAreaManager"));
}

