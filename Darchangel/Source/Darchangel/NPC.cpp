// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject//ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


FVector MidPoint(FVector location1, FVector location2)
{
	FVector Result = FVector((location1.X + location2.X) / 2, (location1.Y + location2.Y) / 2, location1.Z);
	return Result;
}

// Sets default values
ANPC::ANPC() :
	health(max_health), 
	widget_component(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
	//right_fist_collision_box(CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	if (widget_component)
	{
		widget_component->SetupAttachment(RootComponent);
		widget_component->SetWidgetSpace(EWidgetSpace::Screen);
		widget_component->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
		/*static ConstructorHelpers::FClassFinder<UUserWidget> widget_class(TEXT("/Game/UI/HealthBar_BP"));
		if (widget_class.Succeeded())
		{
			widget_component->SetWidgetClass(widget_class.Class);
		}*/
	}
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic* const material_instance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (material_instance)
	{
		material_instance->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, material_instance);
	}

	// attach delegated to the colision box
	if (right_fist_collision_box)
	{
		right_fist_collision_box->OnComponentBeginOverlap.AddDynamic(this, &ANPC::on_attack_overlap_begin);
		right_fist_collision_box->OnComponentEndOverlap.AddDynamic(this, &ANPC::on_attack_overlap_end);
	}
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto const uw = Cast<UHealthBar>(widget_component->GetUserWidgetObject());
	if (uw)
	{
		uw->set_bar_value_percent(health / max_health);
	}

	if (isStun)
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	if (HitByWallOfLight)
	{
		FVector StartTrace = this->GetActorLocation();
		FVector EndTrace = (velocity * DeltaTime) + StartTrace;

		EndTrace.Z += this->GetActorRotation().Pitch;
		SetActorLocation(EndTrace);

		if (countDown >= moveDuration)
		{
			IsLineTranceStart = false;
			HitByWallOfLight = false;
		}
		else
		{
			countDown = countDown + DeltaTime;
		}
	}

	if (IsLineTranceStart)
	{
		FHitResult OutHit;

		FVector Start = this->GetActorLocation();

		FVector ForwardPosition = CharacterFowardPositionWallOfLight;

		FVector End = Start + (ForwardPosition * 100.0f);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

		if (!isHit)
		{
			HitByWallOfLight = true;			
		}	
		/*else
		{
			//print("Hit");
			if (OutHit.Actor->ActorHasTag("Wall"))
			{
				//IsLineTranceStart = false;
				//HitByWallOfLight = false;
				stun(5.0f);
			}
			else
			{
				return;
			}
		}*/
	}

	
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::melee_attack()
{
	if (montage)
	{
		PlayAnimMontage(montage);
	}
}

UAnimMontage* ANPC::get_montage() const
{
	return montage;
}

float ANPC::get_health() const
{
	return health;
}

float ANPC::get_max_health() const
{
	return max_health;
}

void ANPC::set_health(float const new_health)
{
	health = new_health;
	stun(0.5);
	if (health <= 0)
	{
		GetWorld()->DestroyActor(this);
	}
}

void ANPC::HitByWallOfLightFunction(FVector Velocity, float CountDown, float MoveDuration, FVector PlayerForwardPosition)
{
	velocity = Velocity;
	countDown = CountDown;
	moveDuration = MoveDuration;
	CharacterFowardPositionWallOfLight = PlayerForwardPosition;
	IsLineTranceStart = true;
	//HitByWallOfLight = true;
	
}

void ANPC::on_attack_overlap_begin(
	UPrimitiveComponent* const overlapped_component,
	AActor* const other_actor,
	UPrimitiveComponent* other_component,
	int const other_body_index,
	bool const from_sweep,
	FHitResult const& sweep_result)
{
	if (AMainCharacter* const player = Cast<AMainCharacter>(other_actor))
	{
		float const new_health = player->get_health() - player->get_max_health() * 0.05;
		player->set_health(new_health);
	}
}

void ANPC::on_attack_overlap_end(
	UPrimitiveComponent* const overlapped_component,
	AActor* const other_actor,
	UPrimitiveComponent* other_component,
	int const other_body_index)
{

}

void ANPC::stun(float Time)
{
	print("Stun");
	isStun = true;
	GetWorldTimerManager().SetTimer(Handle, this, &ANPC::endStun, Time	, false);
}




void ANPC::HitByGraspofDeathFunction(FVector location)
{
	isStun = true;
	print("Hit by grasp of death");
	playerPos = MidPoint(location, this->GetActorLocation());
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(RootComponent, playerPos, FRotator(0.0f, 0.0f, 0.0f), false, false, 0.2f, false, EMoveComponentAction::Type::Move, LatentInfo);
	GetWorldTimerManager().SetTimer(Handle, this, &ANPC::endStun, 0.5f, false);
}

void ANPC::endStun()
{
	isStun = false;
}



void ANPC::HitByBlessedIdol(float damage)
{
	if (!IsHitByBlessedIdol)
	{
		float const new_health = get_health() - damage;
		set_health(new_health);
		IsHitByBlessedIdol = true;
	}
	GetWorldTimerManager().SetTimer(Handle, this, &ANPC::EndBlessedIdolDamageCD, 1.0f, false);
}

void ANPC::EndBlessedIdolDamageCD()
{
	IsHitByBlessedIdol = false;
}

