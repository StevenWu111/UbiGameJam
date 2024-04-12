// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnLocationUpdateZone.h"

#include "UbisoftGameJamCharacter.h"

// Sets default values
ARespawnLocationUpdateZone::ARespawnLocationUpdateZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

// Called when the game starts or when spawned
void ARespawnLocationUpdateZone::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this,&ARespawnLocationUpdateZone::OnBeginOverlap);
}

// Called every frame
void ARespawnLocationUpdateZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARespawnLocationUpdateZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUbisoftGameJamCharacter* Character = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		Character->RespawnLocation = this->GetActorLocation();
		this->Destroy();
	}
}

