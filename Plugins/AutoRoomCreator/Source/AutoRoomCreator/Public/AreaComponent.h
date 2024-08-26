#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "AreaComponent.generated.h"

class APlaceableActor;
class AFloorAreaManager;

UENUM(BlueprintType)
enum class EGridDivisionMode : uint8
{
	OneDirection UMETA(DisplayName = "One Direction"),
	TwoDirection UMETA(DisplayName = "Two Direction")
};


UENUM(BlueprintType)
enum class EPlacementAlgoType : uint8
{
	GridBased UMETA(DisplayName = "Grid Algorithm"),
	BFDBased UMETA(DisplayName="Best Fit Decreasing Algorithm")
};

UENUM(BlueprintType)
enum class ECallType : uint8
{
	AutoGenerate,
	ManualGenerate
};

UENUM(BlueprintType)
enum class EObjectDirection : uint8
{
	InheritComponentRotation,
	LookAtEachOther,
	LookAtPoint
};

UCLASS(meta = (BlueprintSpawnableComponent))
class AUTOROOMCREATOR_API UAreaComponent : public UBoxComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	TArray<TSubclassOf<APlaceableActor>> placeableObjectsList;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	EPlacementAlgoType placementAlgorithm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	ECallType contentGeneration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	EGridDivisionMode gridDivisionMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	EObjectDirection objectDirection;

private:
	TArray<APlaceableActor*> spawnedPlaceableObjects;
	
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UAreaComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void SetRandomSeed(FRandomStream &newStream);

	void PlaceObjectsInGrid(FRandomStream& newStream);

	void PlaceObjectsByBFD(FRandomStream& newStream);

	void AssignDynamicDelegate(AFloorAreaManager* floorAreaManager);

	void CleanPreviousResult();

	void shuffleList(FRandomStream& newStream);

	void DelayedCall();
};
