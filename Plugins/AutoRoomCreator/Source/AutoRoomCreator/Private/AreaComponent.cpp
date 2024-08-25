// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaComponent.h"
#include "FloorAreaManager.h"
#include "PlaceableActor.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

UAreaComponent::UAreaComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetVisibility(true);
	SetHiddenInGame(false);
}

void UAreaComponent::SetRandomSeed(FRandomStream& newStream)
{
    randomSeed = newStream;
    CleanPreviousResult();
    shuffleList(newStream);
    if (placementAlgorithm == EPlacementAlgoType::BFDBased)
    {
        PlaceObjectsByBFD();
    }
    else
    {
        PlaceObjectsInGrid();
    }
}

void UAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("oooo actor name = %s UAreaComponent = %s  BeginPlay() "),*GetAttachParentActor()->GetName(),*GetName());
    if (contentGeneration == ECallType::AutoGenerate)
    {
        SetRandomSeed(randomSeed);
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

void UAreaComponent::PlaceObjectsInGrid()
{
    int32 placeableListLength = placeableObjectsList.Num();
	if (placeableListLength <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("oooo PlaceActorsInGrid() placeableObjectsList is empty"));
		return;
	}

    FVector boxExtent = GetScaledBoxExtent();
    FRotator boxRotation = GetComponentRotation();
    FVector boxLocation = GetComponentLocation();
    TArray<FVector> gridPositions;

    float boxMaxSizeX = boxExtent.X * 2.0f;
    float boxMaxSizeY = boxExtent.Y * 2.0f;

    // Calculate grid cells (assuming roughly square cells for simplicity)
    int32 gridSize = FMath::CeilToInt(FMath::Sqrt((float)placeableListLength));
    float cellSizeX = boxMaxSizeX / gridSize;
    float cellSizeY = boxMaxSizeY / gridSize;
    //UE_LOG(LogTemp, Warning, TEXT("oooo UAreaComponent PlaceActorsInGrid() gridSize = %d , cellSizeX = %f , cellSizeY = %f "), gridSize, cellSizeX, cellSizeY);
    // Generate grid positions
    for (int32 i = 0; i < placeableListLength; ++i)
    {
        int32 row = i / gridSize;
        int32 col = i % gridSize;

        FVector gridPosition = FVector(col * cellSizeX, row * cellSizeY, boxLocation.Z);
        gridPosition += boxLocation;
        // Rotate the grid position by the yaw of the BoxComponent
        FRotator boxRotationYaw(0.0f, boxRotation.Yaw, 0.0f);
        FVector gridRotatedPosition = UKismetMathLibrary::RotateAngleAxis(gridPosition, -boxRotation.Yaw, FVector(0.0f, 0.0f, 1.0f)) + boxLocation - FVector(boxMaxSizeX / 2.0f, boxMaxSizeY / 2.0f, 0.0f);
        //UE_LOG(LogTemp, Warning, TEXT("oooo UAreaComponent PlaceActorsInGrid() gridPosition = %s"),*gridPosition.ToString());
        gridPositions.Add(gridPosition);
    }

    // Spawn actors considering their sizes
    int32 placeableListIndex = 0;
    for (const FVector& currentPosition : gridPositions)
    {
        if (placeableListIndex >= placeableListLength) 
            break;

        TSubclassOf<APlaceableActor> placeableToSpawn = placeableObjectsList[placeableListIndex];
        FActorSpawnParameters spawnParams;

        APlaceableActor* spawnedPlaceable = GetWorld()->SpawnActor<APlaceableActor>(placeableToSpawn, currentPosition, FRotator(0.0f, 0.0f, 0.0f), spawnParams);

        if (spawnedPlaceable)
        {
            FVector placeableActorSize = spawnedPlaceable->GetActorSize();
            spawnedPlaceable->SetActorLocation(currentPosition - placeableActorSize);
            spawnedPlaceableObjects.Add(spawnedPlaceable);
        }

        placeableListIndex++;
    } 
}

void UAreaComponent::PlaceObjectsByBFD()
{
    int32 placeableListLength = placeableObjectsList.Num();
	if (placeableListLength <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("oooo PlaceActorsInGrid() placeableObjectsList is empty"));
		return;
	}

    FVector boxExtent = GetScaledBoxExtent();
    FVector BoxCenter = GetComponentLocation();

    // Calculate min and max points
    FVector2D areaMin = FVector2D(BoxCenter.X - boxExtent.X, BoxCenter.Y - boxExtent.Y);
    FVector2D areaMax = FVector2D(BoxCenter.X + boxExtent.X, BoxCenter.Y + boxExtent.Y);

    for (TSubclassOf<APlaceableActor> subareaSpawnClass : placeableObjectsList)
    {
        APlaceableActor* parentArea = GetWorld()->SpawnActor<APlaceableActor>(subareaSpawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        spawnedPlaceableObjects.Add(parentArea);
        //UE_LOG(LogTemp, Warning, TEXT("oooo PlaceActorsInGrid() spawned actor name = %s"), *parentArea->GetName());
    }
        FVector2D CurrentPosition = areaMin;

        for (int32 index = 0; index < spawnedPlaceableObjects.Num(); )
        {
            APlaceableActor* subarea = spawnedPlaceableObjects[index];
            if (!subarea)
                continue;

            FVector2D BoxSize(subarea->GetActorSize().X, subarea->GetActorSize().Y);
            FVector2D RotatedBoxSize(subarea->GetActorSize().Y, subarea->GetActorSize().X);  // Size if rotated by 90 degrees

            bool bDoesBoxFit = CurrentPosition.X + BoxSize.X <= areaMax.X &&
                CurrentPosition.Y + BoxSize.Y <= areaMax.Y;

            bool bDoesRotatedBoxFit = CurrentPosition.X + RotatedBoxSize.X <= areaMax.X &&
                CurrentPosition.Y + RotatedBoxSize.Y <= areaMax.Y;
            //UE_LOG(LogTemp, Warning, TEXT("oooo PlaceActorsInGrid() current actor name = %s"), *spawnedPlaceableObjects[index]->GetName());
            if (bDoesBoxFit || bDoesRotatedBoxFit)
            {
                FVector2D fitBoxSize;
                FRotator BoxRotation = FRotator::ZeroRotator;

                if (bDoesBoxFit)
                {
                    fitBoxSize = BoxSize;
                }
                else if (bDoesRotatedBoxFit)
                {
                    fitBoxSize = RotatedBoxSize;
                    BoxRotation = FRotator(0, 90, 0);
                }

                FVector NewLocation(CurrentPosition.X + fitBoxSize.X / 2, CurrentPosition.Y + fitBoxSize.Y / 2, 0.0f);
                subarea->SetActorLocation(NewLocation);
                subarea->SetActorRotation(BoxRotation);
                //UE_LOG(LogTemp, Warning, TEXT("oooo PlaceActorsInGrid() name = %s location = %s"), *subarea->GetName(),*NewLocation.ToString());
                // Update the current position for the next box
                CurrentPosition.X += fitBoxSize.X;

                if (CurrentPosition.X + fitBoxSize.X > areaMax.X)
                {
                    CurrentPosition.X = areaMin.X;
                    CurrentPosition.Y += fitBoxSize.Y;
                }
                index++;
            }
            else
            {
                subarea->Destroy();
                //spawnedPlaceableObjects.Remove(subarea);
                spawnedPlaceableObjects.RemoveAt(index);
                // Handle the case where the box doesn't fit
               // UE_LOG(LogTemp, Warning, TEXT("Box %s does not fit in the defined area"), *subarea->GetName());
            }
        }
}


void UAreaComponent::AssignDynamicDelegate(AFloorAreaManager* floorAreaManager)
{
    if (floorAreaManager)
    {
        floorAreaManager->seedModify.AddDynamic(this, &UAreaComponent::SetRandomSeed);
    }
    //UE_LOG(LogTemp, Warning, TEXT("oooo AssignDynamicDelegate () UAreaComponent"));
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
