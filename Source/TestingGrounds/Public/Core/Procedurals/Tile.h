// Copyright Jose Lloret 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = Navigation)
	FVector NavigationBoundsOffset;

	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	FVector MinExtent;
	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	FVector MaxExtent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Pool)
	void SetPool(UActorPool* InPool);

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1.0f, float MaxScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = Spawning)
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.0f);

private:
	template<class T>
	void RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500.0f, float MinScale = 1.0f, float MaxScale = 1.0f);

	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);

	void PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);

	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	void PositionNavMeshBoundsVolume();

	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

	AActor* NavMeshBoundsVolume;
};
