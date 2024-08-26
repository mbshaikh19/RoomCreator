#include "RoomUI.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"
#include "FloorAreaManager.h"

void URoomUI::NativeConstruct()
{
    Super::NativeConstruct();

    floorAreaManager = GetWorld()->SpawnActor<AFloorAreaManager>(AFloorAreaManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

    if (modifySeedButton)
    {
        modifySeedButton->OnClicked.AddDynamic(this, &URoomUI::OnButtonClicked);
    }
}

void URoomUI::OnButtonClicked()
{
    if (seedInputTextBox)
    {
        int32 newSeed = 0;
        if (FDefaultValueHelper::ParseInt(seedInputTextBox->GetText().ToString(), newSeed))
        {
            if (floorAreaManager->seedModify.IsBound())
            {
                AFloorAreaManager::randomStream.Initialize(newSeed);
                floorAreaManager->seedModify.Broadcast(AFloorAreaManager::randomStream);
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Invalid Seed Input"));
        }
    }
}
