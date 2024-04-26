// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectObject.h"

#include "UbiPlayerController.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ACollectObject::ACollectObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;
}

// Called when the game starts or when spawned
void ACollectObject::BeginPlay()
{
	Super::BeginPlay();
	//Collider->OnComponentBeginOverlap.AddDynamic(this, &ACollectObject::OnBeginOverlap);
}

void ACollectObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AUbisoftGameJamCharacter* Character = Cast<AUbisoftGameJamCharacter>(OtherActor))
	{
		if (VideoUI)
		{
			UUserWidget* WidgetInstance = CreateWidget(GetWorld(), VideoUI);
			WidgetInstance->AddToViewport();
			Character->SetVisibilityNPhysics(false);
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACollectObject::BackToBag, TimeGap, false, TimeGap);
		}
	}
}

void ACollectObject::BackToBag()
{
	if (AUbisoftGameJamCharacter* CurrPlayerController = Cast<AUbisoftGameJamCharacter>( GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		CurrPlayerController->SetVisibilityNPhysics(true);
		this->Destroy();
	}
}

// Called every frame
void ACollectObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

