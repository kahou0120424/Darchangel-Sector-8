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
#include "CableComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Bullet.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "ai_tags.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "HealthBar.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "NPC.h"
#include "BrutalStrike.h"
#include "Chain.h"
#include "GraspofDeath.h"
#include "WallOfLight.h"
#include "BlessedIdol.h"
#include "ChargeParticle.h"
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
	//widget_component(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))),
	Demon_Sword_Collision_Box(CreateDefaultSubobject<UBoxComponent>(TEXT("DemonSwordCollisionBox"))),
	Angel_Sword_Collision_Box(CreateDefaultSubobject<UBoxComponent>(TEXT("AngelSwordCollisionBox"))),
	Cable(CreateDefaultSubobject<UCableComponent>(TEXT("Cable")))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f); //Capsule Size

	bUseControllerRotationPitch = false; // Not allowing the character to rotate
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // allow the charactor ratote to the direction it is moving  
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1200.0f, 0.0f); //rotation rate
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); // name of the component
	CameraBoom->SetupAttachment(RootComponent); // child of the RootComponent


	CameraBoom->TargetArmLength = 300.0f; // how far away the camera from the player
	CameraBoom->bUsePawnControlRotation = true; //rotate the arm base of the controller 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // attact the camera on the end of the boom, let the boom adjust the mass controller rotation of the camera
	FollowCamera->bUsePawnControlRotation = false; // Camera did not rotate relative to the r

	setup_stimulus();


	if (Demon_Sword_Collision_Box)
	{
		FVector const extent(50.0f);
		Demon_Sword_Collision_Box->SetBoxExtent(extent, false);
		Demon_Sword_Collision_Box->SetCollisionProfileName("NoCollision");
		Demon_Sword_Collision_Box->SetGenerateOverlapEvents(false);
	}	
	
	if (Angel_Sword_Collision_Box)
	{
		FVector const extent(50.0f);
		Angel_Sword_Collision_Box->SetBoxExtent(extent, false);
		Angel_Sword_Collision_Box->SetCollisionProfileName("NoCollision");
		Angel_Sword_Collision_Box->SetGenerateOverlapEvents(false);
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	FAttachmentTransformRules const rules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,
		false);

	HideWeapon = false;
	if (Demon_Sword_Collision_Box)
	{

		Demon_Sword_Collision_Box->AttachToComponent(GetMesh(), rules, "Demon_Sword_Middle");
		Demon_Sword_Collision_Box->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));

	}
	if (Angel_Sword_Collision_Box)
	{

		Angel_Sword_Collision_Box->AttachToComponent(GetMesh(), rules, "Dagger_Middle");
		Angel_Sword_Collision_Box->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));

	}

	if (Cable)
	{
		Cable->AttachToComponent(GetMesh(), rules, "index_01_l");
	}

	if (Demon_Sword_Collision_Box)
	{
		Demon_Sword_Collision_Box->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_begin);
		Demon_Sword_Collision_Box->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_end);
	}
	if (Angel_Sword_Collision_Box)
	{
		Angel_Sword_Collision_Box->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_begin);
		Angel_Sword_Collision_Box->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::on_attack_overlap_end);
	}

	isDemon = true;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PullingCoolDownFunction(DeltaTime);
	
	StopCharacter();

	StrongRangeChecker(DeltaTime);
	
	AttackStateCounterFunction(DeltaTime);

	if (IsRangeCharging)
	{
		RotateToMouseCurse();
		BulletSpawnForwardPosition = this->GetActorForwardVector();
		BulletSpawnRotation = this->GetActorRotation();
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("ChainsOfHell", IE_Pressed, this, &AMainCharacter::Raycast);
	PlayerInputComponent->BindAction("Normal Attack", IE_Pressed, this, &AMainCharacter::MeleeAttack);
	PlayerInputComponent->BindAction("Distract", IE_Pressed, this, &AMainCharacter::on_distract);
	PlayerInputComponent->BindAction("Strong Attack", IE_Pressed, this, &AMainCharacter::PlayChargingAnimation);
	PlayerInputComponent->BindAction("Strong Attack", IE_Released, this, &AMainCharacter::PlayStrongAttackAnimation);
	PlayerInputComponent->BindAction("BrutalStrike/Wall Of Light", IE_Pressed, this, &AMainCharacter::BrutalStrikeAnimation);
	PlayerInputComponent->BindAction("Grasp of Death / Blessed Idol", IE_Pressed, this, &AMainCharacter::GraspOfDeathAnimation);
	//PlayerInputComponent->BindAction("Swap Form", IE_Pressed, this, &AMainCharacter::SwapForm);

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
	RotateToMouseCurse();
	if(canJumpWall)
	{
		JumpUp();
	}
}

void AMainCharacter::MeleeAttack() // Melee Attack
{
	if (!ForceStop && !CannotDash)
	{
		RotateToMouseCurse();
		if (isDemon)
		{
			if (atkCount == 0)
			{
				PlayAnimMontage(DemonAttackMontage1, 1.f);
			}
			else if (atkCount == 1)
			{
				PlayAnimMontage(DemonAttackMontage2, 1.f);

			}
			else if (atkCount == 2)
			{
				PlayAnimMontage(DemonAttackMontage3, 1.f);
			
			}
			
		}
		else
		{
			if (atkCount == 0)
			{
				PlayAnimMontage(AngelAttackMontage1, 1.f);
			
			}
			else if (atkCount == 1)
			{
				PlayAnimMontage(AngelAttackMontage2, 1.f);
			}
			
		}
		AttackStateCounter = 0;
		IsAttackState = true;
		ForceStop = true;
	}

}
void AMainCharacter::RangeAttack() // Range Attack
{
	

	if (BulletProjectileClass != NULL)
	{
		IsRangeCharging = false;
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100);
		IsRangeCharging = false;

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			ABullet* Bullet = World->SpawnActor<ABullet>(BulletProjectileClass, SpawnLocation, SpawnRotation);

			FVector NewVelocity = GetActorForwardVector() * 3000.0f;
			Bullet->Velocity = FVector(NewVelocity);
		}
	}
}



void AMainCharacter::setup_stimulus()
{
	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("stimulus"));
	stimulus->RegisterForSense(TSubclassOf < UAISense_Sight>());
	stimulus->RegisterWithPerceptionSystem();
}

void AMainCharacter::on_distract()
{
	if (distraction_sound)
	{
		FVector const loc = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), distraction_sound, loc);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), loc, 1.0f, this, 0.0f, tags::noise_tag);
	}
}

void AMainCharacter::MoveForward(float Axis)
{
	if (!ForceStop )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}


}

void AMainCharacter::MoveRight(float Axis)
{
	if (!ForceStop)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);

	}

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
	if (isDemon)
	{
		Demon_Sword_Collision_Box->SetCollisionProfileName("Sword");
		Demon_Sword_Collision_Box->SetNotifyRigidBodyCollision(true);
		Demon_Sword_Collision_Box->SetGenerateOverlapEvents(true);
	}
	else
	{
		Angel_Sword_Collision_Box->SetCollisionProfileName("Sword");
		Angel_Sword_Collision_Box->SetNotifyRigidBodyCollision(true);
		Angel_Sword_Collision_Box->SetGenerateOverlapEvents(true);
	
	}
	
}

void AMainCharacter::attack_end()
{
	if (isDemon)
	{
		Demon_Sword_Collision_Box->SetCollisionProfileName("Sword");
		Demon_Sword_Collision_Box->SetNotifyRigidBodyCollision(true);
		Demon_Sword_Collision_Box->SetGenerateOverlapEvents(false);
	}
	else
	{
		Angel_Sword_Collision_Box->SetCollisionProfileName("Sword");
		Angel_Sword_Collision_Box->SetNotifyRigidBodyCollision(true);
		Angel_Sword_Collision_Box->SetGenerateOverlapEvents(false);
	}
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
		if (isDemon)
		{
			float const new_health = npc->get_health() - DemonMeleeDamage;
			npc->set_health(new_health);
		}
		else
		{
			float const new_health = npc->get_health() - AngelMeleeDamage;
			npc->set_health(new_health);
		}
		
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
	canJumpWall = true;
	jumpPos = jumpLocation;
}

void AMainCharacter::WallJumpEnd()
{
	canJumpWall = false;
}

void AMainCharacter::JumpUp()
{
	if (!canJumpWall)
		return;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, jumpPos, FRotator(0.0f, 0.0f, 0.0f), false, false, 0.5f, false, EMoveComponentAction::Type::Move, LatentInfo);
}


void AMainCharacter::Rope()
{
	FVector velocity2 = jumpPos - GetActorLocation();

	LaunchCharacter(velocity2 * 7.5, true, true);
}

void AMainCharacter::HideCable()
{
	Cable->SetVisibility(false);
	stopMoving = false;
}


void AMainCharacter::BrutalStrikeAnimation()
{
	if (ForceStop || CannotDash)
		return;
	if (!isDemon)
	{
		PlayWallOfLightAnimation();
		return;
	}
		

	if (!BrutalStrikeInCD)
	{	
		PlayAnimMontage(BrutalStrikeMontage, 1.f, FName("Brutal_Strike_Animation"));
		GetWorldTimerManager().SetTimer(brutalStrikeCDHandle, this, &AMainCharacter::FinishBrutalStrikeCD, BrutalStrikeCD, false);
	}
	
}

void AMainCharacter::BrutalStikeFunction()
{	
	FVector StartPosition = this->GetActorLocation();
	FVector EndPosition = StartPosition + FVector(1.0, 0.0, 0.0);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	TArray<FHitResult> OutHits;
	bool hit;
	if (ShowBrutalStrikeCollisionBox)
	{
		 hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartPosition,
			EndPosition,
			BrutalStrikeHitBoxRadius,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHits,
			true
		);
	}
	else
	{
		 hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartPosition,
			EndPosition,
			BrutalStrikeHitBoxRadius,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true
		);
	}
	

	

	if (hit)
	{
		for (int q = 0; q < OutHits.Num(); q++)
		{
			if (OutHits[q].GetActor() != NULL && OutHits[q].Actor->ActorHasTag("Enemy"))
			{
				if (ANPC* const npc = Cast<ANPC>((OutHits[q].GetActor())))
				{
					float const new_health = npc->get_health() - BrutalStrikeDamage;
					npc->set_health(new_health);
				}
			}
		}
	}

}

void AMainCharacter::GraspOfDeathFunction()
{
	FVector StartPosition = this->GetActorLocation();
	FVector EndPosition = StartPosition + FVector(1.0, 0.0, 0.0);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	TArray<FHitResult> OutHits;
	bool hit;
	if (ShowGrapOfDeathCollisionBox)
	{
			hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartPosition,
			EndPosition,
			GraspOfDeathHitBoxRadius,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			OutHits,
			true
		);
	}
	else
	{
		 hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartPosition,
			EndPosition,
			GraspOfDeathHitBoxRadius,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true
		);
	}

	if (hit)
	{
		for (int q = 0; q < OutHits.Num(); q++)
		{
			if (OutHits[q].GetActor() != NULL && OutHits[q].Actor->ActorHasTag("Enemy"))
			{
				if (ANPC* const npc = Cast<ANPC>((OutHits[q].GetActor())))
				{
					float const new_health = npc->get_health() - GraspOfDeathDamage;
					npc->set_health(new_health);
					npc->HitByGraspofDeathFunction(this->GetActorLocation());
				}
			}
		}
	}
}



void AMainCharacter::WallOfLightFunction()
{
	
	if (WallOfLightProjectile != NULL)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 2);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			AWallOfLight* projectTile = World->SpawnActor<AWallOfLight>(WallOfLightProjectile, SpawnLocation, SpawnRotation);
			FVector NewVelocity = GetActorForwardVector() * WallOfLightFireRate;
			projectTile->Velocity = FVector(NewVelocity);
			projectTile->CharacterForwardPosition = GetActorForwardVector();
			
		}
	}
}

void AMainCharacter::SwapForm()
{
	atkCount = 0;
	if (isDemon)
	{
		isDemon = false;
		PlayAnimMontage(ToAngelMontage, 1.f, FName("ToAngel_Animation"));
	}		
	else
	{
		isDemon = true;
		PlayAnimMontage(ToDemonMontage, 1.f, FName("ToDemon_Animation"));
	}
		
}



void AMainCharacter::FinishBrutalStrikeCD()
{
	BrutalStrikeInCD = false;
}

void AMainCharacter::FinishGraspofDeathCD()
{
	GraspofDeathInCD = false;
}

void AMainCharacter::GraspOfDeathAnimation()
{
	if (ForceStop || CannotDash)
		return;

	if (!isDemon)
	{
		PlayBlessedIdolAnimation();
		
		return;
	}
	
	if (!GraspofDeathInCD )
	{
		PlayAnimMontage(GraspOfDeathMontage, 1.f, FName("Grasp_Of_Death_Animation"));
		GetWorldTimerManager().SetTimer(graspofDeathCDHandle, this, &AMainCharacter::FinishGraspofDeathCD, GraspofDeathCD, false);
	}
}

void AMainCharacter::FinishActtack()
{
	
	ForceStop = false;
	if (atkCount < 2 && isDemon)
	{
		atkCount++;
	}
	else if (!isDemon && atkCount < 1)
	{
		atkCount++;
	}
	else
	{
		atkCount = 0;
	}
	
	
}
	

void AMainCharacter::HideWeaponFunction()
{
	if (HideWeapon)
	{
		HideWeapon = false;
	}
		
	else
	{
		HideWeapon = true;
	}
		

}

void AMainCharacter::CannotDashFunction()
{
	if (CannotDash)
	{
		CannotDash = false;
	}
	else
	{
		CannotDash = true;
	}
}

void AMainCharacter::FinishNormalRangeAnimationFunction()
{
	if (FinishNormalRangeAnimation == true)
	{
		FinishNormalRangeAnimation = false;
	}
	else
	{
		FinishNormalRangeAnimation = true;
	}
	
}

void AMainCharacter::AngelHideSwordFunction()
{
	if (HideAngelWeapon)
	{
		HideAngelWeapon = false;
	}

	else
	{
		HideAngelWeapon = true;
	}
}

void AMainCharacter::BlessedIdolFunction()
{
		if (BlessedIdolProjectile != NULL)
		{
			IsRangeCharging = false;
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = this->GetActorLocation() + (GetActorForwardVector() * 100);

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				ABlessedIdol* projectTile = World->SpawnActor<ABlessedIdol>(BlessedIdolProjectile, SpawnLocation, SpawnRotation);
				FVector NewVelocity = GetActorForwardVector() * BlessedIdolFIreRate;
				projectTile->Velocity = FVector(NewVelocity);
			}		
		}
}

void AMainCharacter::AttackStateCounterFunction(float Deltatime)
{
	if (IsAttackState)
	{		
		AttackStateCounter = AttackStateCounter + Deltatime;
		if (AttackStateCounter >= 5.0)
			EndAttackState();
	}

}

void AMainCharacter::PullingCoolDownFunction(float DeltaTime)
{
	if (isPulling == true)
	{
		pullCD += DeltaTime;
		if (pullCD >= pullDelay)
		{
			isPulling = false;
			pullCD = 0;
		}
	}
}



void AMainCharacter::StopCharacter()
{
	if (stopMoving)
	{
		GetCharacterMovement()->StopMovementImmediately();
	}
}



void AMainCharacter::StrongRangeChecker(float DeltaTime)
{
	if (isRangeHolding)
	{
		rangeHoldTimer += DeltaTime;
	}
	else
	{
		rangeHoldTimer = 0;
	}
}

void AMainCharacter::FinishBlessedIdolCD()
{
	BlessedIdolInCD = false;
}

void AMainCharacter::FinishWallOfLightCD()
{
	WallOfLightInCD = false;
}



void AMainCharacter::EndAttackState()
{
	IsAttackState = false;
}

void AMainCharacter::StrongAttackState()
{
	if (isDemon)
	{
		if (!StrongAttackStateTwo)
		{
			StrongAttackStateTwo = true;
			return;
		}
		else
		{
			StrongAttackStateTwo = false;
			StrongAttackStateThree = true;
		}
	}
	else
	{
		if (!StrongAttackStateOne && !StrongAttackStateTwo)
		{
			StrongAttackStateOne = true;
			return;
		}
		else
		{
			StrongAttackStateOne = false;
			StrongAttackStateTwo = true;

		}
	}
	

}

void AMainCharacter::ComboExpiredFunction()
{
	atkCount = 0;
}

void AMainCharacter::PlayStrongAttackAnimation() // Melee Attack
{
	AttackStateCounter = 0;
	IsAttackState = true;

	if (SpawnedParticle == true)
	{
		ChargePaticleReference->Destroy();
		SpawnedParticle = false;
	}

	if (isDemon)
	{
		if (!IsMeleeCharging)
			return;

		if (StrongAttackStateTwo)
		{
			PlayAnimMontage(StrongAttackStateTwoMontage, 1.0f);

			FVector StartPosition = this->GetActorLocation();
			FVector EndPosition = StartPosition + FVector(1.0, 0.0, 0.0);
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetOwner());
			bool hit;
			TArray<FHitResult> OutHits;

			if (ShowStrongAttack2CollisionBox)
			{
				 hit = UKismetSystemLibrary::SphereTraceMulti(
					GetWorld(),
					StartPosition,
					EndPosition,
					200.0f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForDuration,
					OutHits,
					true
				);
			}
			else
			{
				 hit = UKismetSystemLibrary::SphereTraceMulti(
					GetWorld(),
					StartPosition,
					EndPosition,
					200.0f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::None,
					OutHits,
					true
				);
			}
			

			if (hit)
			{
				for (int q = 0; q < OutHits.Num(); q++)
				{
					if (OutHits[q].GetActor() != NULL && OutHits[q].Actor->ActorHasTag("Enemy"))
					{
						if (ANPC* const npc = Cast<ANPC>((OutHits[q].GetActor())))
						{
							float const new_health = npc->get_health() - StrongAttack2Damage;
							npc->set_health(new_health);
						}
					}
				}
			}
		}
		else if (StrongAttackStateThree)
		{
			PlayAnimMontage(StrongAttackStateThreeMontage, 1.0f);

			FVector StartPosition = this->GetActorLocation();
			FVector EndPosition = StartPosition + FVector(1.0, 0.0, 0.0);
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetOwner());

			TArray<FHitResult> OutHits;
			bool hit;
			if (ShowStrongAttack3CollisionBox)
			{
				 hit = UKismetSystemLibrary::SphereTraceMulti(
					GetWorld(),
					StartPosition,
					EndPosition,
					300.0f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::ForDuration,
					OutHits,
					true
				);
			}
			else
			{
				 hit = UKismetSystemLibrary::SphereTraceMulti(
					GetWorld(),
					StartPosition,
					EndPosition,
					300.0f,
					TraceTypeQuery1,
					false,
					ActorsToIgnore,
					EDrawDebugTrace::None,
					OutHits,
					true
				);
			}
			

			if (hit)
			{
				for (int q = 0; q < OutHits.Num(); q++)
				{
					if (OutHits[q].GetActor() != NULL && OutHits[q].Actor->ActorHasTag("Enemy"))
					{
						if (ANPC* const npc = Cast<ANPC>((OutHits[q].GetActor())))
						{
							float const new_health = npc->get_health() - StrongAttack3Damage;
							npc->set_health(new_health);
						}
					}
				}
			}
		}
		else
		{
			PlayAnimMontage(StrongAttackStateOneMontage, 1.0f);
		}
	}
	else
	{
		if (!IsRangeHold)
			return;

		IsRangeHold = false;
		if (StrongAttackStateTwo)
		{
			PlayAnimMontage(AngelStrongAttack2Montage, 1.0f);

		}
		else if(StrongAttackStateOne)
		{
			PlayAnimMontage(AngelStrongAttack1Montage, 1.0f);
		}
		
	}
	
	StrongAttackStateOne = false;
	StrongAttackStateTwo = false;
	StrongAttackStateThree = false;
	IsMeleeCharging = false;
}

void AMainCharacter::PlayChargingAnimation()
{
	if (ForceStop)
		return;
	if (isDemon)
	{
		PlayAnimMontage(DemonChargeMontage, 1.0f);
		GetWorldTimerManager().SetTimer(ChargeParticleDelay, this, &AMainCharacter::SpawnChargeParticle, 0.4, false);
		ForceStop = true;
	}

	else
	{
		if (!FinishNormalRangeAnimation)
			return;

		PlayRangeAnimation();
		FinishNormalRangeAnimation = false;
		IsRangeHold = true;
	}
		
	
	IsMeleeCharging = true;
}

void AMainCharacter::AngelChargingAnimation()
{
	if (!IsRangeHold)
		return;

	IsRangeCharging = true;
	PlayAnimMontage(AngelChargeMontage, 1.0f);
}

void AMainCharacter::SpawnStrongRangeAttackBullet()
{	
	if (Bullet2ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100);
		IsRangeCharging = false;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			ABullet* Bullet = World->SpawnActor<ABullet>(Bullet2ProjectileClass, SpawnLocation, SpawnRotation);

			FVector NewVelocity = GetActorForwardVector() * 2000.0f;
			Bullet->Velocity = FVector(NewVelocity);
		
		}

	}
}

void AMainCharacter::SpawnStrongRangeAttackBullet2()
{
	if (Bullet3ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetActorRotation();
		const FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * 100);
		IsRangeCharging = false;
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			ABullet* Bullet = World->SpawnActor<ABullet>(Bullet3ProjectileClass, SpawnLocation, SpawnRotation);

			FVector NewVelocity = GetActorForwardVector() * 2000.0f;
			Bullet->Velocity = FVector(NewVelocity);
		;
		}

	}
}

void AMainCharacter::PlayBlessedIdolAnimation()
{
	if (BlessedIdolInCD)
		return;
	PlayAnimMontage(BlessedIdolMontage, 1.0f);
	RotateToMouseCurse();
	GetWorldTimerManager().SetTimer(BlessedIdolCDHandle, this, &AMainCharacter::FinishBlessedIdolCD, BleesedIdolCooldown, false);
}

void AMainCharacter::PlayWallOfLightAnimation()
{
	if (WallOfLightInCD)
		return;
	RotateToMouseCurse();
	PlayAnimMontage(WallOfLightMontage, 1.0f);
	GetWorldTimerManager().SetTimer(WallOfLightHandle, this, &AMainCharacter::FinishWallOfLightCD, WallOfLightCooldown, false);
}

void AMainCharacter::PlayRangeAnimation()
{
	IsRangeHold = true;
	IsRangeCharging = true;
	PlayAnimMontage(AngelNormalRangeAttackMontage, 1.0f);
}

void AMainCharacter::SpawnChargeParticle()
{
	if (!IsMeleeCharging)
		return;

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - 80.0f);

	UWorld* const World = GetWorld();

	if (World != NULL)
	{
		ChargePaticleReference = World->SpawnActor<AChargeParticle>(ChargeParticle, SpawnLocation, SpawnRotation);
		SpawnedParticle = true;
	}
}
