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
#include "Bullet.h"
#include "Perception/AISense_Sight.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "HealthBar.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "NPC.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

///////////////////////////////////////////////////////////////////////
//Distance Formula
double Distance(FVector a, FVector b)
{
	double distance = abs(sqrt(pow(b.X - a.X, 2) + pow(b.Y - a.Y, 2)));
	return distance;
}
///////////////////////////////////////////////////////////////////////

// Sets default values
AMainCharacter::AMainCharacter() :
	health(max_health),
	widget_component(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))),
	sword_collision_box(CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollisionBox")))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //Capsule Size

	bUseControllerRotationPitch = false; // Not allowing the character to rotate
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // allow the charactor ratote to the direction it is moving  
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f); //rotation rate
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
	if (widget_component)
	{
		widget_component->SetupAttachment(RootComponent);
		widget_component->SetWidgetSpace(EWidgetSpace::Screen);
		widget_component->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
		static ConstructorHelpers::FClassFinder<UUserWidget> widget_class(TEXT("/Game/UI/HealthBar_BP"));
		if (widget_class.Succeeded())
		{
			widget_component->SetWidgetClass(widget_class.Class);
		}
	}

	if (sword_collision_box)
	{
		FVector const extent(50.0f);
		sword_collision_box->SetBoxExtent(extent, false);
		sword_collision_box->SetCollisionProfileName("NoCollision");
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (sword_collision_box)
	{
		FAttachmentTransformRules const rules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);
		sword_collision_box->AttachToComponent(GetMesh(), rules, "FX_Sword_Top");
		sword_collision_box->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
	}

	if (sword_collision_box)
	{
		sword_collision_box->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_begin);
		sword_collision_box->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_end);
	}
}

void AMainCharacter::RotateToMouseCurse() 
{
	APlayerController* playerController = (APlayerController*)GetWorld()->GetFirstPlayerController();
	bool isHit2 = playerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, OutHit2);
	FRotator rotatePoint = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OutHit2.Location);
	this->SetActorRotation(FRotator(0, rotatePoint.Yaw, 0));
}

void AMainCharacter::Raycast() //Chain Of Hell
{
	if (!isPulling && !canJumpWall)
	{
		isPulling = true;		
		RotateToMouseCurse();
		FVector Start = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z);		
		FVector ForwardVector = this->GetActorForwardVector();

		Start = Start + ForwardVector;
		FVector End = Start + (ForwardVector * 5000.0f);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(ActorHasTag("Wall"));

		//Draw raycast
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
		
		if (isHit)
		{
			if (OutHit.Actor->ActorHasTag("Enemy"))
			{
				isPull = true;
				velocity = (this->GetActorLocation() - OutHit.Actor->GetActorLocation()) * 0.005;
				playerPos = this->GetActorLocation();
			}

			else if (OutHit.Actor->ActorHasTag("GraspOfDeath"))
			{
				gotPull = true;
				velocity = FVector(this->GetActorForwardVector().X, this->GetActorForwardVector().Y, 0);
				wallPos = OutHit.Actor->GetActorLocation();
			}
			
		}
	}

	else
	{
		JumpUp();
	}
	


}

void AMainCharacter::MeleeAttack() // Melee Attack
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
	}
}

void AMainCharacter::RangeAttack() // Range Attack
{
	

		if (ProjectileClass != NULL)
		{
			//const FRotator SpawnRotation = GetControlRotation();
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				ABullet* Bullet = World->SpawnActor<ABullet>(ProjectileClass, SpawnLocation, SpawnRotation);

				FVector NewVelocity = GetActorForwardVector() * 2000.0f;
				Bullet->Velocity = FVector(NewVelocity);
			}

		}
		
	

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto const uw = Cast<UHealthBar>(widget_component->GetUserWidgetObject());
	if (uw)
	{
		uw->set_bar_value_percent(health / max_health);
	}

	if (isPull == true)
	{
		float distance = Distance(playerPos, OutHit.Actor->GetActorLocation());				
		FString TheFloatStr = FString::SanitizeFloat(distance);	
		if (distance >= DistanceBetweenActors)
		{
			OutHit.Actor->AddActorLocalOffset((velocity * speed ), false, 0, ETeleportType::None);
		}
		else
		{
			isPull = false;
		}
	}

	if (gotPull == true)
	{
		float distance = abs(Distance(wallPos,GetActorLocation()));
		FString TheFloatStr = FString::SanitizeFloat(distance);
		if (distance >= DistanceBetweenActors)
		{
			LaunchCharacter(velocity * gotPullSpeed, true, true);
		}
		else
		{
			gotPull = false;
		}
		
	}

	if (isAttacking == true)
	{
		atkCD += DeltaTime;
		if (atkCD >= AttackDelay)
		{
			isAttacking = false;
			atkCD = 0;
		}

	}
	if (isPulling == true)
	{
		pullCD += DeltaTime;
		if (pullCD >= pullDelay)
		{
			isPulling = false;
			pullCD = 0;
		}
	}

	if (isShooting)
	{
		i ++;
		RotateToMouseCurse();
		if (i >= bulletRate)
		{
			RangeAttack();
			i = 0;
		}
	}

	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("ChainsOfHell", IE_Pressed, this, &AMainCharacter::Raycast);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacter::Dash);
	//PlayerInputComponent->BindAction("Dash", IE_Released, this, &AMainCharacter::StopDash);

	PlayerInputComponent->BindAction("Normal Attack", IE_Pressed, this, &AMainCharacter::MeleeAttack);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMainCharacter::Fire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AMainCharacter::FinishFire);
}

void AMainCharacter::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf < UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMainCharacter::MoveForward(float Axis)
{
	if (!isAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
	
	
}

void AMainCharacter::MoveRight(float Axis)
{
	if (!isAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);

	}
	
}


void AMainCharacter::Dash()
{
	if (canDash)
	{
			
		LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * dashDistance, true, true);
		canDash = false;
		GetWorldTimerManager().SetTimer(dashHandle, this, &AMainCharacter::ResetDash, dashCooldown, false);

	}
}

void AMainCharacter::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 100.0f;
	GetWorldTimerManager().SetTimer(dashHandle, this, &AMainCharacter::ResetDash, dashCooldown, false);
}

void AMainCharacter::ResetDash()
{
	canDash = true;
}

void AMainCharacter::AttackMove() // Move forward while attacking
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	LaunchCharacter(FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0).GetSafeNormal() * attackForwardDistance, true, true);
}

void AMainCharacter::Fire()
{
	isShooting = true;
}

void AMainCharacter::FinishFire()
{
	isShooting = false;
}

float AMainCharacter::get_health() const
{
	return health;
}

float AMainCharacter::get_max_health() const
{
	return max_health;
}

void AMainCharacter::set_health(float const new_health)
{
	health = new_health;
	if (health <= 0)
	{
		auto const controller = UGameplayStatics::GetPlayerController(this, 0);
		controller->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->MovementState.bCanJump = false;
	}
}

void AMainCharacter::attack_start()
{
	sword_collision_box->SetCollisionProfileName("Sword");
	sword_collision_box->SetNotifyRigidBodyCollision(true);
}

void AMainCharacter::attack_end()
{
	sword_collision_box->SetCollisionProfileName("NoCollision");
	sword_collision_box->SetNotifyRigidBodyCollision(false);
}

void AMainCharacter::on_attack_overlap_begin(
	UPrimitiveComponent* const overlapped_component,
	AActor* const other_actor,
	UPrimitiveComponent* other_component,
	int const other_body_index,
	bool const from_sweep,
	FHitResult const& sweep_result)
{
	if (ANPC* const npc = Cast<ANPC>(other_actor))
	{
		print("Hit");
		float const new_health = npc->get_health() - npc->get_max_health() * 0.5f;
		npc->set_health(new_health);
	}
}

void AMainCharacter::on_attack_overlap_end(
	UPrimitiveComponent* const overlapped_component,
	AActor* const other_actor,
	UPrimitiveComponent* other_component,
	int const other_body_index)
{

}

void AMainCharacter::WallJumpStart(FVector jumpLocation)
{
	print("Overlap Begin");
	canJumpWall = true;
	jumpPos = jumpLocation;

	
}

void AMainCharacter::WallJumpEnd()
{
	print("Overlap End");
	canJumpWall = false;
}


void AMainCharacter::JumpUp()
{
	if (!canJumpWall)
		return;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, jumpPos, FRotator(0.0f, 0.0f, 0.0f), false, false, 1.0f, false, EMoveComponentAction::Type::Move, LatentInfo);
}

