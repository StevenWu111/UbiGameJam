// Fill out your copyright notice in the Description page of Project Settings.


#include "BirdNest.h"

#include "Components/BoxComponent.h"

// Sets default values
ABirdNest::ABirdNest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComponent;
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABirdNest::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ABirdNest::OnBeginOverlap);
}

void ABirdNest::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

// Called every frame
void ABirdNest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

