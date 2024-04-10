// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectObject.h"

#include "CharacterAfterLeap.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ACollectObject::ACollectObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComponent;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectObject::BeginPlay()
{
	Super::BeginPlay();
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectObject::OnBeginOverlap);
}

void ACollectObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacterAfterLeap* Character = Cast<ACharacterAfterLeap>(OtherActor))
	{
		if (VideoUI)
		{
			UUserWidget* WidgetInstance = CreateWidget(GetWorld(), VideoUI);
			WidgetInstance->AddToViewport();
		}
	}
}

// Called every frame
void ACollectObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

