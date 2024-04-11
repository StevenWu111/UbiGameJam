// Fill out your copyright notice in the Description page of Project Settings.


#include "UbiPlayerController.h"

void AUbiPlayerController::SwitchCharacter(bool bSwitchToBagCharacter)
{
	this->UnPossess();
	if (bSwitchToBagCharacter)
	{
		if (PlasticBagCharacter)
		{
			this->Possess(PlasticBagCharacter);
		}
	}
	else
	{
		if (CharacterWithPlasticBag)
		{
			this->Possess(CharacterWithPlasticBag);
		}
	}
}

void AUbiPlayerController::SetupCharactersWhenSwitchBack(FVector BagLocation)
{
	if (PlasticBagCharacter && CharacterWithPlasticBag)
	{
		Location = BagLocation;
		PlasticBagCharacter->SetLookOutComponent(nullptr);
		PlasticBagCharacter->SetVisibilityNPhysics(false);
		PlasticBagCharacter->SetMeshLocation(Location);
		PlasticBagCharacter->SetVisibilityNPhysics(true);
		SwitchCharacter(true);
		//GetWorldTimerManager().SetTimer(Timer, this, &AUbiPlayerController::SetBagCharacterLocation, 1, false, 1);
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

void AUbiPlayerController::SetBagCharacterLocation()
{
	
}
