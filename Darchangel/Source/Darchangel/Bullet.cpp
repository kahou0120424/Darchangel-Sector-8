// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine.h"
#include "NPC.h"

// Sets default values 
ABullet::ABullet() : BulletCollision(CreateDefaultSubobject<UBoxComponent>(TEXT("BulletCollision")))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;


	if (BulletCollision)
	{
		FVector const extent(50.0f);
		BulletCollision->SetBoxExtent(extent, false);
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	

	if (BulletCollision)
	{
		BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::on_overlap_begin);
	}
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult hitResult;

	FVector StartTrace = this->GetActorLocation();
	FVector EndTrace = (Velocity * DeltaTime) + StartTrace;
	EndTrace.Z += this->GetActorRotation().Pitch;

	FCollisionQueryParams CollisonParams;
	CollisonParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, StartTrace, EndTrace, ECC_Destructible, CollisonParams))
	{
		if (hitResult.GetActor() && hitResult.GetActor()->ActorHasTag("Enemy"))
		{
			ANPC* npc = Cast<ANPC>(hitResult.GetActor());
			float const new_health = npc->get_health() - npc->get_max_health() * percentageDamage;
			npc->set_health(new_health);	

		}
		
		Destroy();
	}
	
	else
	{
		BulletExpiry += DeltaTime;

		SetActorLocation(EndTrace);

	}

	if (BulletExpiry > 3)
	{
		Destroy();
	}

}

void ABullet::on_overlap_begin(UPrimitiveComponent* const overlapped_component, AActor* const other_actor, UPrimitiveComponent* other_component, int const other_body_index, bool const from_sweep, FHitResult const& sweep_result)
{
}

