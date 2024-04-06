// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Logging/LogMacros.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/LevelSequence/Public/LevelSequence.h"
#include "UbisoftGameJamCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AUbisoftGameJamCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Quit Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* QuiteInteractAction;
	
	UPROPERTY(EditAnywhere)
	double ApproachingSpeed = 70.f;

	UPROPERTY(EditAnywhere)
	double RotateSpeed = 1.0f;


	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* MeshComponent;

public:
	AUbisoftGameJamCharacter();

	void RemoveJumpMove();

	void ResetJumpMove();

	void MoveToTargetLocation(UStaticMeshComponent* TargetMeshRef, float DeltaTime);

	void CreateUI(TSubclassOf<UUserWidget> CreateUI);

	void RemoveUI();
	

	/*
	 * Getters and Setters
	 */
	AActor* GetCurrInteractActor();
	void SetCurrInteractActor(AActor* NewActor);

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void JumpUp(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);
	void QuiteInteraction(const FInputActionValue& Value);

	AActor* CurrInteractActor;

	//Use those two to unbind the Move and jump function
	FEnhancedInputActionEventBinding* MoveBinding;
	FEnhancedInputActionEventBinding* JumpBinding;

	UEnhancedInputComponent* EnhancedInputComponent;

	//Use this as the target to make our character approach to the look out
	UStaticMeshComponent* LookOutTargetMesh;

	//Use this to store a ref of the UI that we created before
	UUserWidget* WidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LeapUI;

	bool bIsReadyToLeap = false;

	UPROPERTY(EditAnywhere)
	ULevelSequence* LeapSeq;

	
	UFUNCTION()
	void OnSequenceStop();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void Tick(float DeltaSeconds) override;
};

