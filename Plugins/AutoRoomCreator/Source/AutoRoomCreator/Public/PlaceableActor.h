#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "AreaComponent.h"
#include "PlaceableActor.generated.h"

UCLASS()
class AUTOROOMCREATOR_API APlaceableActor : public AActor
{
	GENERATED_BODY()
	
public:	

	APlaceableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	USceneComponent* sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	UAreaComponent* areaComponent;

public:

	FVector GetStaticMeshComponentSize();

	FVector GetBoxComponentSize();

	FVector GetActorSize();
};
