// Fill out your copyright notice in the Description page of Project Settings.


#include "WallOfLight.h"
#include "NPC.h"

// Sets default values
AWallOfLight::AWallOfLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnActorBeginOverlap.AddDynamic(this, &AWallOfLight::OnOverlapBegin);

	OnActorEndOverlap.AddDynamic(this, &AWallOfLight::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AWallOfLight::BeginPlay()
{
	Super::BeginPlay();
	timeUp = false;
}

void AWallOfLight::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ANPC* const npc = Cast<ANPC>(OtherActor))
	{
		npc->HitByWallOfLightFunction(Velocity, countDown, moveDuration, CharacterForwardPosition);
		float const new_health = npc->get_health() - Damage;
		npc->set_health(new_health);
	}
}

void AWallOfLight::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}

// Called every frame
void AWallOfLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (countDown >= moveDuration)
	{
		timeUp = true;

		/*if (lifeTimeCounter >= Life)
		{
			Destroy();
		}*/

		lifeTimeCounter = lifeTimeCounter + DeltaTime;
		return;
	}

	if (!timeUp)
	{
		FVector StartTrace = this->GetActorLocation();
		FVector EndTrace = (Velocity * DeltaTime) + StartTrace;

		EndTrace.Z += this->GetActorRotation().Pitch;
		SetActorLocation(EndTrace);
	}

	countDown = countDown + DeltaTime;
}

