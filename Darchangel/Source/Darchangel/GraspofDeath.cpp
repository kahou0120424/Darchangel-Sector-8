// Fill out your copyright notice in the Description page of Project Settings.


#include "GraspofDeath.h"
#include "NPC.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


// Sets default values
AGraspofDeath::AGraspofDeath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OnActorBeginOverlap.AddDynamic(this, &AGraspofDeath::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &AGraspofDeath::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AGraspofDeath::BeginPlay()
{
	Super::BeginPlay();
	lifeTime = 0.5;

}
void AGraspofDeath::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ANPC* const npc = Cast<ANPC>(OtherActor))
	{
		print("Hit by Grasp Of Death");
		float const new_health = npc->get_health() - npc->get_max_health() * Damage;
		npc->set_health(new_health);
		npc->HitByGraspofDeathFunction(this->GetActorLocation());
	}
}
void AGraspofDeath::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}
// Called every frame
void AGraspofDeath::Tick(float DeltaTime)
{
	if (lifeTime >= 0)
	{
		lifeTime = lifeTime - DeltaTime;
	}
	else
	{
		Destroy();
	}
}

