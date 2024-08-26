#include "AreaComponent.h"
#include "FloorAreaManager.h"
#include "PlaceableActor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"

UAreaComponent::UAreaComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetVisibility(true);
	SetHiddenInGame(true);
}

void UAreaComponent::SetRandomSeed(FRandomStream& newStream)
{
    CleanPreviousResult();
    shuffleList(AFloorAreaManager::randomStream);
    if (placementAlgorithm == EPlacementAlgoType::BFDBased)
    {
        PlaceObjectsByBFD(AFloorAreaManager::randomStream);
    }
    else
    {
        PlaceObjectsInGrid(AFloorAreaManager::randomStream);
    }
}

void UAreaComponent::BeginPlay()
{
	Super::BeginPlay();
    if (contentGeneration == ECallType::AutoGenerate)
    {
        FTimerHandle timerHandle;
        GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UAreaComponent::DelayedCall, 0.2f, false);
    }
}

void UAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAreaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
    CleanPreviousResult();
}


void UAreaComponent::PlaceObjectsInGrid(FRandomStream& newStream)
{
    int32 placeableListLength = placeableObjectsList.Num();
    if (placeableListLength <= 0)
    {
        return;
    }

    for (TSubclassOf<APlaceableActor> placeableActorSpawnClass : placeableObjectsList)
    {
        APlaceableActor* actor = GetWorld()->SpawnActor<APlaceableActor>(placeableActorSpawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        spawnedPlaceableObjects.Add(actor);
    }

    FVector boxExtent = GetScaledBoxExtent();
    FRotator boxRotation = GetComponentRotation();
    FVector boxLocation = GetComponentLocation();

    float boxMaxSizeX = boxExtent.X * 2.0f;
    float boxMaxSizeY = boxExtent.Y * 2.0f;

    int32 gridSizeX = 1;
    int32 gridSizeY = 1;

    int32 sizeOffsetX = 0;
    int32 sizeOffsetY = 0;

    if (gridDivisionMode == EGridDivisionMode::TwoDirection)
    {
        gridSizeX = FMath::CeilToInt(FMath::Sqrt((float)placeableListLength));
        gridSizeY = FMath::CeilToInt((float)placeableListLength / gridSizeX);

        sizeOffsetX = (boxMaxSizeX / gridSizeX) * 0.5f;
        sizeOffsetY = (boxMaxSizeY / gridSizeY) * 0.5f;
    }
    else if (gridDivisionMode == EGridDivisionMode::OneDirection)
    {
        if (boxExtent.X > boxExtent.Y)
        {
            gridSizeX = placeableListLength;
            sizeOffsetX = (boxMaxSizeX / gridSizeX) * 0.5f;
            
        }
        else
        {
            gridSizeY = placeableListLength;            
            sizeOffsetY = (boxMaxSizeY / gridSizeY) * 0.5f;
        }
    }

    float cellSizeX = boxMaxSizeX / gridSizeX;
    float cellSizeY = boxMaxSizeY / gridSizeY;

    for (int32 i = 0; i < placeableListLength; ++i)
    {
        int32 row = 0;
        int32 col = 0;
        if (sizeOffsetX == 0 && sizeOffsetY > 0)
        {
            row = i / gridSizeY;
            col = i % gridSizeY;
        }
        else if (sizeOffsetY == 0 && sizeOffsetX > 0)
        {
            row = i / gridSizeX;
            col = i % gridSizeX;
        }
        else
        {
            row = i / gridSizeY;
            col = i % gridSizeY;
        }
        
        FVector gridPosition = FVector(row * cellSizeX, col * cellSizeY, 0.0f);
        if (placeableListLength != 1)
            gridPosition -= FVector(sizeOffsetX , sizeOffsetY, 0.0f);

        FVector rotatedGridPosition = UKismetMathLibrary::RotateAngleAxis(gridPosition, boxRotation.Yaw , FVector(0.0f, 0.0f, 1.0f)) + boxLocation;

        spawnedPlaceableObjects[i]->SetActorLocation(rotatedGridPosition);

        if (objectDirection == EObjectDirection::InheritComponentRotation)
        {
            spawnedPlaceableObjects[i]->SetActorRotation(boxRotation);
        }
        else if (objectDirection == EObjectDirection::LookAtEachOther)
        {
            if (i % 2 == 0)
            {
                spawnedPlaceableObjects[i]->SetActorRotation(boxRotation);
            }
            else
            {
                int32 previousIndex = i - 1;
                FVector previousObjectLoc = spawnedPlaceableObjects[previousIndex]->GetActorLocation();
                FVector objectDifferenceVect = previousObjectLoc - rotatedGridPosition;
                FRotator currentObjectDirection = objectDifferenceVect.Rotation();

                float yaw = AFloorAreaManager::randomStream.FRandRange(currentObjectDirection.Yaw - 100.0f, currentObjectDirection.Yaw - 80.0f);
                spawnedPlaceableObjects[i]->SetActorRotation(FRotator(boxRotation.Pitch, yaw, boxRotation.Roll));
                objectDifferenceVect = rotatedGridPosition - previousObjectLoc;
                currentObjectDirection = objectDifferenceVect.Rotation();

                yaw = AFloorAreaManager::randomStream.FRandRange(currentObjectDirection.Yaw - 100.0f, currentObjectDirection.Yaw - 80.0f);
                spawnedPlaceableObjects[previousIndex]->SetActorRotation(FRotator(boxRotation.Pitch, yaw, boxRotation.Roll));
            }
        }
        else if (objectDirection == EObjectDirection::LookAtPoint)
        {
            FVector parentObjectLoc = GetOwner()->GetActorLocation();
            FVector objectDifferenceVect = parentObjectLoc - rotatedGridPosition;
            FRotator currentObjectDirection = objectDifferenceVect.Rotation();
            float yaw = AFloorAreaManager::randomStream.FRandRange(currentObjectDirection.Yaw - 100.0f, currentObjectDirection.Yaw - 80.0f);
            spawnedPlaceableObjects[i]->SetActorRotation(FRotator(boxRotation.Pitch, yaw, boxRotation.Roll));
        }
    }
}


void UAreaComponent::PlaceObjectsByBFD(FRandomStream& newStream)
{
    int32 placeableListLength = placeableObjectsList.Num();
	if (placeableListLength <= 0)
	{
		return;
	}

    FVector boxExtent = GetScaledBoxExtent();
    FVector boxLocation = GetComponentLocation();

    FVector2D areaMin = FVector2D(boxLocation.X - boxExtent.X, boxLocation.Y - boxExtent.Y);
    FVector2D areaMax = FVector2D(boxLocation.X + boxExtent.X, boxLocation.Y + boxExtent.Y);

    for (TSubclassOf<APlaceableActor> placeableActorSpawnClass : placeableObjectsList)
    {
        APlaceableActor* spawnedActor = GetWorld()->SpawnActor<APlaceableActor>(placeableActorSpawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        spawnedPlaceableObjects.Add(spawnedActor);
    }
        FVector2D currentPosition = areaMin;

        for (int32 index = 0; index < spawnedPlaceableObjects.Num(); )
        {
            APlaceableActor* currentSpawnedActor = spawnedPlaceableObjects[index];
            if (!currentSpawnedActor)
                continue;

            FVector2D BoxSize(currentSpawnedActor->GetActorSize().X, currentSpawnedActor->GetActorSize().Y);
            FVector2D RotatedBoxSize(currentSpawnedActor->GetActorSize().Y, currentSpawnedActor->GetActorSize().X);

            bool bDoesBoxFit = currentPosition.X + BoxSize.X <= areaMax.X &&
                currentPosition.Y + BoxSize.Y <= areaMax.Y;

            bool bDoesRotatedBoxFit = currentPosition.X + RotatedBoxSize.X <= areaMax.X &&
                currentPosition.Y + RotatedBoxSize.Y <= areaMax.Y;

            if (bDoesBoxFit || bDoesRotatedBoxFit)
            {
                FVector2D fitBoxSize;
                FRotator fitBoxOrientation = FRotator::ZeroRotator;

                if (bDoesBoxFit)
                {
                    fitBoxSize = BoxSize;
                }
                else if (bDoesRotatedBoxFit)
                {
                    fitBoxSize = RotatedBoxSize;
                    fitBoxOrientation = FRotator(0, 90, 0);
                }

                FVector newLocation(currentPosition.X + fitBoxSize.X / 2, currentPosition.Y + fitBoxSize.Y / 2, 0.0f);
                currentSpawnedActor->SetActorLocation(newLocation);
                currentSpawnedActor->SetActorRotation(fitBoxOrientation);
                currentPosition.X += fitBoxSize.X;

                if (currentPosition.X + fitBoxSize.X > areaMax.X)
                {
                    currentPosition.X = areaMin.X;
                    currentPosition.Y += fitBoxSize.Y;
                }
                index++;
            }
            else
            {
                currentSpawnedActor->Destroy();
                spawnedPlaceableObjects.RemoveAt(index);
            }
        }
}


void UAreaComponent::AssignDynamicDelegate(AFloorAreaManager* floorAreaManager)
{
    if (floorAreaManager)
    {
        floorAreaManager->seedModify.AddDynamic(this, &UAreaComponent::SetRandomSeed);
    }
}

void UAreaComponent::CleanPreviousResult()
{
    for (auto spawnedActor : spawnedPlaceableObjects)
    {
        if (spawnedActor)
            spawnedActor->Destroy();
    }
    spawnedPlaceableObjects.Empty();
}

void UAreaComponent::shuffleList(FRandomStream& newStream)
{
    int32 size = placeableObjectsList.Num();
    if (size <= 1)
    {
        return;
    }

    for (int32 indexA = size - 1; indexA > 0; --indexA)
    { 
        int32 indexB = newStream.RandHelper(indexA);
        placeableObjectsList.Swap(indexA, indexB);
    }
}

void UAreaComponent::DelayedCall()
{
    SetRandomSeed(AFloorAreaManager::randomStream);
}
