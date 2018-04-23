// Copyright Jose Lloret 2018

#include "ActorPool.h"
#include "GameFramework/Actor.h"


UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = true;

}

AActor* UActorPool::Checkout()
{
	if (Pool.Num() != 0)
	{
		return Pool.Pop();
	}

	return nullptr;
}

void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	Pool.Push(ActorToAdd);
}
