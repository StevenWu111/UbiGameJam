// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAfterLeap.h"
#include "UbisoftGameJamCharacter.h"
#include "GameFramework/PlayerController.h"
#include "UbiPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UBISOFTGAMEJAM_API AUbiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SwitchCharacter(bool bSwitchToBagCharacter);

	void SetupCharactersWhenSwitchBack(FVector BagLocation);

	void SetCharacterWithPlasticBag(ACharacterAfterLeap* CharacterInWorld);

	FTimerHandle Timer;

	void SetBagCharacterLocation();

	FVector Location;

protected:
	AUbisoftGameJamCharacter* PlasticBagCharacter;
	
	ACharacterAfterLeap* CharacterWithPlasticBag;

	virtual void BeginPlay() override;;

	int FoundNum = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIEndClass;

	UUserWidget* UIEnd;
};


