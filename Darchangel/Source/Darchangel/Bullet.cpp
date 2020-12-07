// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine.h"
#include "NPC.h"

// Sets default values 
ABullet::ABullet() 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
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

	FVector StartPosition = this->GetActorLocation();
	FVector EndPosition = StartPosition + FVector(0.0f, 0.0f, 0.1f);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> OutHits;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, StartTrace, EndTrace, ECC_Destructible, CollisonParams))
	{
		if (hitResult.GetActor() && hitResult.GetActor()->ActorHasTag("Enemy"))
		{
			ANPC* npc = Cast<ANPC>(hitResult.GetActor());
			float const new_health = npc->get_health() - Damage;
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
	
	/*if (HitEnemy)
	{
		bool hit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			StartPosition,
			EndPosition,
			100,
			TraceTypeQuery1,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true
		);

		if (hit)
		{
			for (int q = 0; q < OutHits.Num(); q++)
			{
				if (OutHits[q].GetActor() != NULL && OutHits[q].Actor->ActorHasTag("Enemy"))
				{
					if (ANPC* const npc = Cast<ANPC>((OutHits[q].GetActor())))
					{
						float const new_health = npc->get_health() - Damage;
						npc->set_health(new_health);
					}
				}
			}
		}
		if (IsMaxChargeArrow)
		{
			SetActorLocation(EndTrace);
			BulletExpiry += DeltaTime;
		}
		else
		{
			Destroy();
		}
		
	}
	else
	{
		BulletExpiry += DeltaTime;

		SetActorLocation(EndTrace);

	}

	

	if (HitWall)
	{
		Destroy();
	}*/

}



