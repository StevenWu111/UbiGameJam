// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CollectObject.generated.h"

UCLASS()
class UBISOFTGAMEJAM_API ACollectObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectObject();

	UPROPERTY(EditAnywhere)
	USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> VideoUI;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent*OverlappedComponent, AActor*OtherActor,UPrimitiveComponent*OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
