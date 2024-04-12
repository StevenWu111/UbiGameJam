// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "RespawnLocationUpdateZone.generated.h"

UCLASS()
class UBISOFTGAMEJAM_API ARespawnLocationUpdateZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawnLocationUpdateZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	
	UFUNCTION()
void OnBeginOverlap(UPrimitiveComponent*OverlappedComponent, AActor*OtherActor,UPrimitiveComponent*OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);


};
