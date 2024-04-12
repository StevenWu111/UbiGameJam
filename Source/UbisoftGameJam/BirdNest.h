// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "BirdNest.generated.h"

UCLASS()
class UBISOFTGAMEJAM_API ABirdNest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABirdNest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Collider;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent*OverlappedComponent, AActor*OtherActor,UPrimitiveComponent*OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);



	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CollectUI;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BackToBag();

	UPROPERTY(EditAnywhere)
	float TimeGap = 5;
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	ULevelSequence* NestDropSeq;

};


