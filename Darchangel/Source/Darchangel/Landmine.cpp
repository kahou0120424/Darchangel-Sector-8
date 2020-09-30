// Fill out your copyright notice in the Description page of Project Settings.


#include "Landmine.h"
#include "NPC.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
ALandmine::ALandmine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALandmine::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ALandmine::OnOverlapBegin);
}

// Called every frame
void ALandmine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALandmine::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor != NULL && OtherActor->ActorHasTag("Enemy"))
	{
		if (ANPC* const npc = Cast<ANPC>(OtherActor))
		{
			float const new_health = npc->get_health() - npc->get_max_health() * Damage;
			npc->set_health(new_health);
		}
	}
}

