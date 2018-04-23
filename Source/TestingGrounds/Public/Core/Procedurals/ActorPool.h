// Copyright Jose Lloret 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTINGGROUNDS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:
	UActorPool();
	
	AActor* Checkout();

	void Return(AActor* ActorToReturn);

	void Add(AActor* ActorToAdd);

private:
	TArray<AActor*> Pool;

};
