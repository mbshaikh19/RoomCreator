// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Blueprint/UserWidget.h"
#include "RoomUI.generated.h"

/**
 * 
 */
UCLASS()
class AUTOROOMCREATOR_API URoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
    FRandomStream newStream;

public:
    virtual void NativeConstruct() override;

protected:
    class AFloorAreaManager* floorAreaManager;

    UPROPERTY(meta = (BindWidget))
    UButton* modifySeedButton;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* seedInputTextBox;

    UFUNCTION()
    void OnButtonClicked();
};