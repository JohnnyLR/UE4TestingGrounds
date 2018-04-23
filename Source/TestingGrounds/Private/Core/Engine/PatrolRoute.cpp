// Copyright Jose Lloret 2018

#include "PatrolRoute.h"


AActor* UPatrolRoute::GetPatrolPoint(int32 Index) const
{
	if (PatrolPointsLength() == 0) { return GetOwner(); }

	return ToPatrolPoints[Index];
}

int32 UPatrolRoute::PatrolPointsLength() const
{
	return ToPatrolPoints.Num();
}
