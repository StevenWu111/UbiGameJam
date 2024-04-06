// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAfterLeap.h"
#include "InteractInterface.h"
#include "UbisoftGameJamCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "LeapArea.generated.h"

UCLASS()
class UBISOFTGAMEJAM_API ALeapArea : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALeapArea();

	//The function from Interface, use to cancel the selection shader
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Selection")
	void Interact();
	//Make it be able to override in BP
	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractUI;

	UPROPERTY(EditAnywhere)
	ACharacterAfterLeap* CharacterAfterLeap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USphereComponent* Collider;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComponent;
	
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent*OverlappedComponent, AActor*OtherActor,UPrimitiveComponent*OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	AUbisoftGameJamCharacter* PlayerCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
