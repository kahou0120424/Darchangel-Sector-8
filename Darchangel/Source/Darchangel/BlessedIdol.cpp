// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine.h"
#include "NPC.h"
#include "BlessedIdol.h"
#include "Landmine.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
// Sets default values
ABlessedIdol::ABlessedIdol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlessedIdol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlessedIdol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BulletRangeCounter += DeltaTime;
	FHitResult hitResult;

	FVector StartTrace = this->GetActorLocation();
	FVector EndTrace = (Velocity * DeltaTime) + StartTrace;
	EndTrace.Z += this->GetActorRotation().Pitch;

	FCollisionQueryParams CollisonParams;
	CollisonParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, StartTrace, EndTrace, ECC_Destructible, CollisonParams))
	{
		if (hitResult.GetActor() && hitResult.GetComponent()->ComponentHasTag("Enemy"))
		{
			FVector StartPosition = this->GetActorLocation();
			FVector EndPosition = StartPosition + FVector(0.0f, 0.0f, 0.1f);
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetOwner());

			TArray<FHitResult> OutHits;

			if (BulletRangeCounter < 0.2f)
			{
				AoeRange = AoeRange1;
			}
			else if(BulletRangeCounter >= 0.2 && BulletRangeCounter<= 0.5 )
			{
				AoeRange = AoeRange2;
			}
			else
			{
				AoeRange = AoeRange3;
			}

			bool hit = UKismetSystemLibrary::SphereTraceMulti(
				GetWorld(),
				StartPosition,
				EndPosition,
				AoeRange,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
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
							print("Hit");
							float const new_health = npc->get_health() - npc->get_max_health() * 0.5f;
							npc->set_health(new_health);
							
						}
					}
				}
			}
		}

		Destroy();
	}

	else
	{
		BulletExpiry += DeltaTime;

		SetActorLocation(EndTrace);

		//Velocity += FVector(0.f, 0.f, -200.0f) * DeltaTime;
	}

	if (BulletExpiry > 1.5)
	{
		if (LandMineProjectile != NULL)
		{
			const FRotator SpawnRotation = GetActorRotation();
			const FVector SpawnLocation = GetActorLocation();

			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				ALandmine* projectile = World->SpawnActor<ALandmine>(LandMineProjectile, SpawnLocation, SpawnRotation);
			}
		}

		Destroy();
	}

}

