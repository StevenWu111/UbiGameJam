// Fill out your copyright notice in the Description page of Project Settings.



#include "CantEnterZone.h"


// Sets default values
ACantEnterZone::ACantEnterZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

// Called when the game starts or when spawned
void ACantEnterZone::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACantEnterZone::OnBeginOverlap);
	Collider->OnComponentEndOverlap.AddDynamic(this, &ACantEnterZone::ACantEnterZone::OnEndOverlap);
}

void ACantEnterZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUbisoftGameJamCharacter* CurrPlayerCharacter = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		CurrPlayerCharacter->OverlappingInvalidZoneCount++;
		CurrPlayerCharacter->EnterInvalidZone();
	}
}

void ACantEnterZone::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AUbisoftGameJamCharacter* CurrPlayerCharacter = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		CurrPlayerCharacter->OverlappingInvalidZoneCount--;
		CurrPlayerCharacter->ExitInvalidZone();
	}
}

// Called every frame
void ACantEnterZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

