// Fill out your copyright notice in the Description page of Project Settings.


#include "Chain.h"
#include "Engine.h"
#include "NPC.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

// Sets default values
AChain::AChain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	MyComp->SetNotifyRigidBodyCollision(true);

	MyComp->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	MyComp->OnComponentHit.AddDynamic(this, &AChain::OnCompHit);
	RootComponent = MyComp;
}

// Called when the game starts or when spawned
void AChain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChain::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherActor->ActorHasTag("Enemy")))
	{
		ANPC* npc = Cast<ANPC>(OtherActor);
		npc->stun(0.5f);
	}
}


