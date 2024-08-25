// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	APlaceableActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	USceneComponent* sceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	UStaticMeshComponent* staticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placeable")
	UAreaComponent* areaComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetStaticMeshComponentSize();

	FVector GetBoxComponentSize();

	FVector GetActorSize();
};
