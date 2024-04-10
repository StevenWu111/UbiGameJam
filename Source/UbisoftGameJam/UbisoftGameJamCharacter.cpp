// Copyright Epic Games, Inc. All Rights Reserved.

#include "UbisoftGameJamCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "InteractInterface.h"
#include "LevelSequenceActor.h"
#include "UbiPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/LevelSequence/Public/LevelSequencePlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUbisoftGameJamCharacter

AUbisoftGameJamCharacter::AUbisoftGameJamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComponent;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(MeshComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	
}


void AUbisoftGameJamCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	MeshComponent->OnComponentHit.AddDynamic(this, &AUbisoftGameJamCharacter::OnComponentHit);

	RespawnLocation = this->GetActorLocation();

	CurrTimerCount = RespawnTime;
}

void AUbisoftGameJamCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	const float Speed = MeshComponent->GetPhysicsLinearVelocity().Length();
	GEngine->AddOnScreenDebugMessage(-1,2.0f,FColor::Red,FString::Printf(TEXT("Speed is %f"),Speed));
	if (LookOutTargetMesh)
	{
		MoveToTargetLocation(LookOutTargetMesh, DeltaSeconds);
	}
	if (InvalidZoneTimerHandle.IsValid())
	{
		CurrTimerCount -= DeltaSeconds;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUbisoftGameJamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent) {
		
		// Jumping
		JumpBinding = &EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::JumpUp);

		// Moving
		MoveBinding = &EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::Move);
		EnhancedInputComponent->BindAction(MoveBackAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::MoveBack);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::MoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::MoveRight);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUbisoftGameJamCharacter::Look);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::Interact);
		EnhancedInputComponent->BindAction(QuiteInteractAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::QuiteInteraction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AUbisoftGameJamCharacter::Move(const FInputActionValue& Value)
{
	if (MeshComponent)
	{
		const FVector ImpulseLocation = FollowCamera->GetComponentLocation();
		//FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(500);
		//DrawDebugSphere(GetWorld(), ImpulseLocation, ExplosionSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0,0);
		MeshComponent->AddRadialImpulse(ImpulseLocation, 600, 120,RIF_Linear, true);
	}
}

void AUbisoftGameJamCharacter::MoveBack(const FInputActionValue& Value)
{
	if (MeshComponent)
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		FVector Direction = MeshComponent->GetComponentLocation() - CameraLocation;
		Direction.Normalize();
		const FVector ImpulseLocation = MeshComponent->GetComponentLocation() + Direction * 100;
		FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(200);
		DrawDebugSphere(GetWorld(), ImpulseLocation, ExplosionSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0,0);
		MeshComponent->AddRadialImpulse(ImpulseLocation, 200, 100,RIF_Linear, true);
	}
}

void AUbisoftGameJamCharacter::MoveLeft(const FInputActionValue& Value)
{
	if (MeshComponent)
	{
		FVector Direction = -FollowCamera->GetRightVector();
		Direction.Normalize();
		const FVector ImpulseLocation = MeshComponent->GetComponentLocation() + Direction * 100;
		FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(200);
		DrawDebugSphere(GetWorld(), ImpulseLocation, ExplosionSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0,0);
		MeshComponent->AddRadialImpulse(ImpulseLocation, 200, 100,RIF_Linear, true);
	}
}

void AUbisoftGameJamCharacter::MoveRight(const FInputActionValue& Value)
{
	if (MeshComponent)
	{
		FVector Direction = FollowCamera->GetRightVector();
		Direction.Normalize();
		const FVector ImpulseLocation = MeshComponent->GetComponentLocation() + Direction * 100;
		FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(200);
		DrawDebugSphere(GetWorld(), ImpulseLocation, ExplosionSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0,0);
		MeshComponent->AddRadialImpulse(ImpulseLocation, 200, 100,RIF_Linear, true);
	}
}

void AUbisoftGameJamCharacter::JumpUp(const FInputActionValue& Value)
{
	if (MeshComponent)
	{
		const FVector ImpulseLocation = MeshComponent->GetComponentLocation() - FVector(0,0,200);
		//FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(400);
		//DrawDebugSphere(GetWorld(), ImpulseLocation, ExplosionSphere.GetSphereRadius(), 50, FColor::Red, false, 2.0f, 0,0);
		MeshComponent->AddRadialImpulse(ImpulseLocation, 400, 130,RIF_Linear, true);
	}
}

void AUbisoftGameJamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUbisoftGameJamCharacter::Interact(const FInputActionValue& Value)
{
	if (bIsReadyToLeap && LeapSeq)
	{
		ALevelSequenceActor* SequenceActor;
		if (ULevelSequencePlayer* SeqPlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),LeapSeq,FMovieSceneSequencePlaybackSettings(), SequenceActor))
		{
			MeshComponent->SetVisibility(false);
			SeqPlayer->OnFinished.AddDynamic(this, &AUbisoftGameJamCharacter::OnSequenceStop);
			SeqPlayer->Play();
		}
		this->RemoveUI();
		return;
	}
	if (CurrInteractActor)
	{
		if (CurrInteractActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(CurrInteractActor);
		}
	}
}

void AUbisoftGameJamCharacter::QuiteInteraction(const FInputActionValue& Value)
{
	if (bIsReadyToLeap)
	{
		bIsReadyToLeap = false;
		this->RemoveUI();
		MeshComponent->SetSimulatePhysics(true);
		this->ResetJumpMove();
		this->JumpUp(Value);
	}
}


void AUbisoftGameJamCharacter::RemoveJumpMove()
{
	if (EnhancedInputComponent)
	{
		GetWorld()->GetFirstPlayerController()->InputComponent;
		EnhancedInputComponent->RemoveBindingByHandle(JumpBinding->GetHandle());
		EnhancedInputComponent->RemoveBindingByHandle(MoveBinding->GetHandle());
	}
}

void AUbisoftGameJamCharacter::ResetJumpMove()
{
	if (EnhancedInputComponent)
	{
		// Jumping
		JumpBinding = &EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::JumpUp);

		// Moving
		MoveBinding = &EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AUbisoftGameJamCharacter::Move);
	}
}

void AUbisoftGameJamCharacter::MoveToTargetLocation(UStaticMeshComponent* TargetMeshRef, float DeltaTime)
{
	if (LookOutTargetMesh)
	{
		const FVector CurrLocation =MeshComponent->GetComponentLocation();
		const FVector TargetLocation = TargetMeshRef->GetComponentLocation();
		const FRotator TargetRotation = TargetMeshRef->GetComponentRotation();
		FVector Direction = TargetLocation - CurrLocation;
		if (Direction.Length() <= 0.3 && TargetRotation.Equals(MeshComponent->GetComponentRotation(), 1))
		{
			RemoveUI();
			CreateUI(LeapUI);
			LookOutTargetMesh = nullptr;
			bIsReadyToLeap = true;
			return;
		}
		Direction.Normalize();
		const FVector NextLocation = CurrLocation + Direction * ApproachingSpeed * DeltaTime;
		const FRotator NextRotation = UKismetMathLibrary::RInterpTo(MeshComponent->GetComponentRotation(),TargetRotation, DeltaTime, RotateSpeed);
		MeshComponent->SetWorldLocation(NextLocation);
		MeshComponent->SetWorldRotation(NextRotation);
	}
	else
	{
		LookOutTargetMesh = TargetMeshRef;
		MeshComponent->SetSimulatePhysics(false);
	}
}

void AUbisoftGameJamCharacter::CreateUI(TSubclassOf<UUserWidget> CreateUI)
{
	if (CreateUI)
	{
		WidgetInstance = CreateWidget(GetWorld(), CreateUI);
	}
	if (WidgetInstance)
	{
		WidgetInstance->AddToViewport();
	}
}

void AUbisoftGameJamCharacter::RemoveUI()
{
	if (WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
}

void AUbisoftGameJamCharacter::EnterInvalidZone()
{
	if (!InvalidZoneTimerHandle.IsValid())
	{
		GetWorldTimerManager().SetTimer(InvalidZoneTimerHandle,this,&AUbisoftGameJamCharacter::Respawn,RespawnTime,false,RespawnTime);
		this->CreateUI(InvalidZoneUI);
	}
}

void AUbisoftGameJamCharacter::ExitInvalidZone()
{
	if (OverlappingInvalidZoneCount <= 0)
	{
		//Make sure it back to zero
		OverlappingInvalidZoneCount = 0;
		GetWorldTimerManager().ClearTimer(InvalidZoneTimerHandle);
		RemoveUI();
		CurrTimerCount = RespawnTime;
	}
}

void AUbisoftGameJamCharacter::Respawn()
{
	if (!RespawnLocation.IsZero())
	{
		MeshComponent->SetSimulatePhysics(false);
		MeshComponent->SetWorldLocation(RespawnLocation);
		MeshComponent->SetSimulatePhysics(true);
		RemoveUI();
		CurrTimerCount = RespawnTime;
	}
}


/*
 * Getters and Setters
 */
AActor* AUbisoftGameJamCharacter::GetCurrInteractActor()
{
	return CurrInteractActor;
}

void AUbisoftGameJamCharacter::SetCurrInteractActor(AActor* NewActor)
{
	CurrInteractActor = NewActor;
}

void AUbisoftGameJamCharacter::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
}


void AUbisoftGameJamCharacter::OnSequenceStop()
{
	if (AUbiPlayerController* CurrPlayerController = Cast<AUbiPlayerController>(this->GetController()))
	{
		CurrPlayerController->SwitchCharacter(false);
	}
}


