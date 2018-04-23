// Copyright Jose Lloret 2018

#include "Tile.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"
#include "AI/Navigation/NavigationSystem.h"


ATile::ATile()
{
	PrimaryActorTick.bCanEverTick = true;

	NavigationBoundsOffset = FVector(2000.0f, 0.0f, 0.0f);
	MinExtent = FVector(0.0f, -2000.0f, 0.0f);
	MaxExtent = FVector(4000.0f, 2000.0f, 0.0f);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Pool != NULL && NavMeshBoundsVolume != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("El Pool [%s] recupera el NavMeshVolume [%s]"), *Pool->GetName(), *NavMeshBoundsVolume->GetName());
		Pool->Return(NavMeshBoundsVolume);
	}
	else
	{
		if (Pool != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("El Pool [%s] no recupera ningun NavMeshVolume"), *Pool->GetName());
		}
	}
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool* InPool)
{
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1.0f, 1.0f);
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);

	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::FRandRange(MinScale, MaxScale);

		bool FoundEmptyLocation = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);

		if (FoundEmptyLocation)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.0f, 180.0f);
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);

	if (Spawned != NULL) {
		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	}
}

void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);

	if (Spawned != NULL) {
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0.0f, SpawnPosition.Rotation, 0.0f));
		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Guard"));
	}
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds(MinExtent, MaxExtent);
	const int MAX_ATTEMPTS = 100;

	for (size_t i = 0; i < MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}

	return false;
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("El Tile [%s] posiciona el NavMeshVolume [%s] extraido del Pool [%s]"), *GetName(), *NavMeshBoundsVolume->GetName(), *Pool->GetName());
		NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
		GetWorld()->GetNavigationSystem()->Build();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("El Tile [%s] se ha quedado sin NavMesVolume. Duplica otro NavMeshVolume"), *GetName(), *Pool->GetName());
	}
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);

	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	/*
	FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), Location, 0.0f, Radius, FQuat::Identity, ResultColor, true, 100.0f);
	*/

	return !HasHit;
}
