// Fill out your copyright notice in the Description page of Project Settings.


#include "UbiPlayerController.h"

void AUbiPlayerController::SwitchCharacter(bool bSwitchToBagCharacter)
{
	if (bSwitchToBagCharacter)
	{
		if (PlasticBagCharacter)
		{
			this->UnPossess();
			this->Possess(PlasticBagCharacter);
		}
	}
	else
	{
		if (CharacterWithPlasticBag)
		{
			this->UnPossess();
			this->Possess(CharacterWithPlasticBag);
		}
	}
}

void AUbiPlayerController::SetCharacterWithPlasticBag(ACharacterAfterLeap* CharacterInWorld)
{
	CharacterWithPlasticBag = CharacterInWorld;
}

void AUbiPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlasticBagCharacter = Cast<AUbisoftGameJamCharacter>(this->GetPawn());
}
