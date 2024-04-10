// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UbisoftGameJamCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CantEnterZone.generated.h"

UCLASS()
class UBISOFTGAMEJAM_API ACantEnterZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACantEnterZone();

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent*OverlappedComponent, AActor*OtherActor,UPrimitiveComponent*OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	AUbisoftGameJamCharacter* PlayerCharacter;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
