#include "PlaceableActor.h"


APlaceableActor::APlaceableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene Component"));
	sceneComponent->RegisterComponent();
	RootComponent = sceneComponent;

	staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	staticMeshComponent->SetupAttachment(sceneComponent);

	areaComponent = CreateDefaultSubobject<UAreaComponent>(TEXT("Area Component"));
	areaComponent->SetupAttachment(sceneComponent);
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
	if (meshCompSize.Length() > boxCompSize.Length())
	{
		return meshCompSize;
	}
	return boxCompSize * 2.0f;
}

