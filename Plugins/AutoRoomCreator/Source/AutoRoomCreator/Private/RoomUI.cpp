// Fill out your copyright notice in the Description page of Project Settings.


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
    //UE_LOG(LogTemp, Warning, TEXT("oooo OnButtonClicked() step 01"));
    if (seedInputTextBox)
    {
        //UE_LOG(LogTemp, Warning, TEXT("oooo OnButtonClicked() step 02"));
        int32 newSeed = 0;
        if (FDefaultValueHelper::ParseInt(seedInputTextBox->GetText().ToString(), newSeed))
        {
           // UE_LOG(LogTemp, Warning, TEXT("oooo OnButtonClicked() step 03"));
            if (floorAreaManager->seedModify.IsBound())
            {
                newStream.Initialize(newSeed);
                //UE_LOG(LogTemp, Warning, TEXT("oooo OnButtonClicked() step 04"));
                floorAreaManager->seedModify.Broadcast(newStream);
            }
        }
        else {
            //UE_LOG(LogTemp, Warning, TEXT("oooo OnButtonClicked() step 05"));
        }
    }

}
