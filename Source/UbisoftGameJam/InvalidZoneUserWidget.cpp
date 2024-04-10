// Fill out your copyright notice in the Description page of Project Settings.


#include "InvalidZoneUserWidget.h"
#include "Components/TextBlock.h"

void UInvalidZoneUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Player = Cast<AUbisoftGameJamCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UInvalidZoneUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (Player)
	{
		CurrTimer->SetText(FText::AsNumber(Player->CurrTimerCount));
	}
}
