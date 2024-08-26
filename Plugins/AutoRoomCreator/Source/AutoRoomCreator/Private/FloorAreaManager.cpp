#include "FloorAreaManager.h"
#include "AreaComponent.h"
#include "PlaceableActor.h"
#include "Kismet/GameplayStatics.h"

AFloorAreaManager* AFloorAreaManager::floorAreaManager;

FRandomStream AFloorAreaManager::randomStream;

AFloorAreaManager::AFloorAreaManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFloorAreaManager::BeginPlay()
{
	Super::BeginPlay();
	floorAreaManager = this;
	seedModify.AddDynamic(this, &AFloorAreaManager::SeedModified);

	APlaceableActor* placeableActor = Cast<APlaceableActor>(UGameplayStatics::GetActorOfClass(GetWorld(), APlaceableActor::StaticClass()));
	if (placeableActor)
	{
		UAreaComponent* areaComponent = placeableActor->GetComponentByClass<UAreaComponent>();
		if (areaComponent)
		{
			areaComponent->AssignDynamicDelegate(this);
		}
	}
}

void AFloorAreaManager::SeedModified(FRandomStream& newStream)
{
	//UE_LOG(LogTemp, Warning, TEXT("oooo Seed modified () FloorAreaManager seed value = %d"), AFloorAreaManager::randomStream);
}

