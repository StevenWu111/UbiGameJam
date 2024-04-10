// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UbisoftGameJamCharacter.h"
#include "Blueprint/UserWidget.h"
#include "InvalidZoneUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UBISOFTGAMEJAM_API UInvalidZoneUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrTimer;
	
	AUbisoftGameJamCharacter* Player; 
	void NativeConstruct () override;
	void NativeTick (const FGeometry& MyGeometry, float InDeltaTime) override;
};
