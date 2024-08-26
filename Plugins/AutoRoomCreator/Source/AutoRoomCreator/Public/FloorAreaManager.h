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

	virtual void BeginPlay() override;

public:	

	AFloorAreaManager();

	UFUNCTION()
	void SeedModified(FRandomStream& newStream);
};
