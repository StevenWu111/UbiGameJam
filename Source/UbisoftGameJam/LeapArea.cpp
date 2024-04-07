// Fill out your copyright notice in the Description page of Project Settings.

#include "LeapArea.h"

#include "UbiPlayerController.h"
#include "UbisoftGameJamCharacter.h"

// Sets default values
ALeapArea::ALeapArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
}

void ALeapArea::Interact_Implementation()
{
	IInteractInterface::Interact_Implementation();
	if (PlayerCharacter)
	{
		PlayerCharacter->RemoveJumpMove();
		PlayerCharacter->MoveToTargetLocation(MeshComponent, 0.1);
		PlayerCharacter->RemoveUI();
	}
}

// Called when the game starts or when spawned
void ALeapArea::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this,&ALeapArea::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ALeapArea::OnEndOverlap);
	MeshComponent->SetVisibility(false);
}

void ALeapArea::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUbisoftGameJamCharacter* Player = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		if (InteractUI)
		{
			Player->CreateUI(InteractUI);
		}
		PlayerCharacter = Player;
		Player->SetCurrInteractActor(this);

		if (AUbiPlayerController* PlayerController = Cast<AUbiPlayerController>(Player->GetController()))
		{
			PlayerController->SetCharacterWithPlasticBag(CharacterAfterLeap);
		}
	}
}

void ALeapArea::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (AUbisoftGameJamCharacter* Player = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		PlayerCharacter = nullptr;
		Player->SetCurrInteractActor(nullptr);
		Player->RemoveUI();
	}
}

// Called every frame
void ALeapArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

