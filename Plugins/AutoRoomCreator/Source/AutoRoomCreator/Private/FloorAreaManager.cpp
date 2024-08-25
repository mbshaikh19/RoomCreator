// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorAreaManager.h"
#include "AreaComponent.h"
#include "PlaceableActor.h"
#include "Kismet/GameplayStatics.h"

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
	UE_LOG(LogTemp, Warning, TEXT(" oooo AFloorAreaManager BeginPlay() step 01"));
	seedModify.AddDynamic(this, &AFloorAreaManager::SeedModified);

	APlaceableActor* placeableActor = Cast<APlaceableActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APlaceableActor::StaticClass()));
	if (placeableActor)
	{
		UE_LOG(LogTemp, Warning, TEXT(" oooo AFloorAreaManager BeginPlay() step 03"));
		UAreaComponent* areaComponent = placeableActor->GetComponentByClass<UAreaComponent>();
		if (areaComponent)
		{
			areaComponent->AssignDynamicDelegate(this);
			UE_LOG(LogTemp, Warning, TEXT(" oooo AFloorAreaManager BeginPlay() step 04"));
		}
	}
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

