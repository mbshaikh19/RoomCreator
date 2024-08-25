// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableActor.h"

// Sets default values
APlaceableActor::APlaceableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	sceneComponent->RegisterComponent();
	RootComponent = sceneComponent;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	staticMeshComponent->SetupAttachment(sceneComponent);

	areaComponent = CreateDefaultSubobject<UAreaComponent>(TEXT("Area Component"));
	areaComponent->SetupAttachment(sceneComponent);
}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APlaceableActor::GetStaticMeshComponentSize()
{
	if (staticMeshComponent)
	{
		if (staticMeshComponent->GetStaticMesh())
		{
			FVector size = staticMeshComponent->GetComponentScale() * staticMeshComponent->Bounds.BoxExtent;
			return size / 2.0f;
		}
	}
	return FVector::ZeroVector;
}

FVector APlaceableActor::GetBoxComponentSize()
{
	if (areaComponent)
	{
		return areaComponent->GetScaledBoxExtent();
	}
	return FVector::ZeroVector;
}

FVector APlaceableActor::GetActorSize()
{
	FVector meshCompSize = GetStaticMeshComponentSize();
	FVector boxCompSize = GetBoxComponentSize();
	//if ((meshCompSize.X * meshCompSize.Y) > (boxCompSize.X * boxCompSize.Y))
	if (meshCompSize.Length() > boxCompSize.Length())
	{
		//UE_LOG(LogTemp, Warning, TEXT("oooo meshCompSize = %s  boxCompSize = %s step 01 "), *meshCompSize.ToString(),*boxCompSize.ToString());
		return meshCompSize;
	}
	//UE_LOG(LogTemp, Warning, TEXT("oooo meshCompSize = %s  boxCompSize = %s step 02 "), *meshCompSize.ToString(), *boxCompSize.ToString());
	return boxCompSize * 2.0f;
}

