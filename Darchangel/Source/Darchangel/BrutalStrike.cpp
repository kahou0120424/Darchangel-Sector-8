// Fill out your copyright notice in the Description page of Project Settings.


#include "BrutalStrike.h"
#include "DrawDebugHelpers.h"
#include "NPC.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


// Sets default values
ABrutalStrike::ABrutalStrike()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &ABrutalStrike::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ABrutalStrike::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ABrutalStrike::BeginPlay()
{
	Super::BeginPlay();
}

void ABrutalStrike::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ANPC* const npc = Cast<ANPC>(OtherActor))
	{
		print("Hit by Brutal Strike");
		float const new_health = npc->get_health() - npc->get_max_health() * 0.5f;
		npc->set_health(new_health);
	}
}

void ABrutalStrike::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{

}

void ABrutalStrike::setLifeTime(float LifeTime)
{
	lifeTime = LifeTime;
}

// Called every frame
void ABrutalStrike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (lifeTime >= 0)
	{
		lifeTime = lifeTime - DeltaTime;
	}
	else
	{
		Destroy();
	}
}


