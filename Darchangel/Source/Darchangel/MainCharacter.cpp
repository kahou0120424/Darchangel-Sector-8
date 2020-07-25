// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "cmath"
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

///////////////////////////////////////////////////////////////////////
//Distance Formula
double Distance(FVector a, FVector b)
{
	double distance = abs(sqrt(pow(b.X - a.X, 2) + pow(b.Y - a.Y, 2)));
	return distance;
}
///////////////////////////////////////////////////////////////////////

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //Capsule Size

	bUseControllerRotationPitch = false; // Not allowing the character to rotate
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // allow the charactor ratote to the direction it is moving  
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); //rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // name of the component
	CameraBoom->SetupAttachment(RootComponent); // child of the RootComponent


	CameraBoom->TargetArmLength = 300.0f; // how far away the camera from the player
	CameraBoom->bUsePawnControlRotation = true; //rotate the arm base of the controller 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attact the camera on the end of the boom, let the boom adjust the mass controller rotation of the camera
	FollowCamera->bUsePawnControlRotation = false; // Camera did not rotate relative to the r

	// Attack Animation
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObject(TEXT("AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryA_Montage.Attack_PrimaryA_Montage'"));
	if (AttackMontageObject.Succeeded())
	{
		AttackMontage = AttackMontageObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObject2(TEXT("AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryB_Montage.Attack_PrimaryB_Montage'"));
	if (AttackMontageObject.Succeeded())
	{
		AttackMontage2 = AttackMontageObject2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMontageObject3(TEXT("AnimMontage'/Game/ParagonGreystone/Characters/Heroes/Greystone/Animations/Attack_PrimaryC_Montage.Attack_PrimaryC_Montage'"));
	if (AttackMontageObject.Succeeded())
	{
		AttackMontage3 = AttackMontageObject3.Object;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	canDash = true;
	dashDistance = 6000.0f;
	dashCooldown = 1.0f;
	dashStop = 0.1f;

	setup_stimulus();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMainCharacter::Raycast()
{

	FVector Start = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, 150);
	APlayerController* playerController = (APlayerController*)GetWorld()->GetFirstPlayerController();
	bool isHit2 = playerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, OutHit2);
	FRotator rotatePoint = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OutHit2.Location);
	this->SetActorRotation(FRotator(0, rotatePoint.Yaw, 0));
	FVector ForwardVector = this->GetActorForwardVector();

	Start = Start + ForwardVector;
	FVector End = Start + (ForwardVector * 5000.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ActorHasTag("Wall"));

	//Draw raycast
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
	//bool isHit = GetWorld()->LineTraceMultiByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{		
		if (OutHit.Actor->ActorHasTag("Enemy"))
		{
			isPull = true;
			playerPos = this->GetActorLocation();
			targetLocation = OutHit.Actor->GetActorLocation();
			velocity = -GetActorForwardVector();
		}
		
	}


}

void AMainCharacter::AttackStart()
{
	if (!isAttacking)
	{
		if (atkCount == 0)
		{
			PlayAnimMontage(AttackMontage, 1.f, FName("Attack_PrimaryA"));
			atkCount++;
		}
		else if (atkCount == 1)
		{
			PlayAnimMontage(AttackMontage2, 1.f, FName("Attack_PrimaryB"));
			atkCount++;
		}
		else
		{
			PlayAnimMontage(AttackMontage3, 1.f, FName("Attack_PrimaryC"));
			atkCount = 0;
		}

		isAttacking = true;
		atkCD = 0;
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isPull == true)
	{
		float distance = Distance(playerPos, OutHit.Actor->GetActorLocation());		
		timer = speed; // no acceloration		
		percent = timer / seconds;
		velocity += velocity * percent;
		FString TheFloatStr = FString::SanitizeFloat(distance);	
		if (distance >= 200)
		{
			OutHit.Actor->AddActorLocalOffset((velocity), false, 0, ETeleportType::None);
		}
		else
		{
			isPull = false;
			percent = 0;
			timer = 0;
		}
	}

	if (isAttacking == true)
	{
		atkCD += DeltaTime;
		if (atkCD >= AttackDelay)
		{
			isAttacking = false;
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("ChainsOfHell", IE_Pressed, this, &AMainCharacter::Raycast);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);

	PlayerInputComponent->BindAction("Normal Attack", IE_Pressed, this, &AMainCharacter::AttackStart);
}

void AMainCharacter::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf < UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMainCharacter::MoveForward(float Axis)
{
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
	
}

void AMainCharacter::MoveRight(float Axis)
{
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
	
}


void AMainCharacter::Dash()
{
	if (canDash)
	{
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * dashDistance, true, true);
		canDash = false;
		GetWorldTimerManager().SetTimer(dashHandle, this, &AMainCharacter::ResetDash, dashCooldown, false);
	}
}

void AMainCharacter::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 100000000.0f;
	GetWorldTimerManager().SetTimer(dashHandle, this, &AMainCharacter::ResetDash, dashCooldown, false);
}

void AMainCharacter::ResetDash()
{
	canDash = true;
}



