#pragma once

#include "CoreMinimal.h"
#include "HomingMissileDataTypes.generated.h"

UENUM(BlueprintType)
enum EQueenCommandType
{
	Attack,
	Collect,
	Retreat,
	None
};

UENUM(BlueprintType)
enum EEntityTeam : uint8
{
	Bee,
	Wasp,
	Pollen
};

USTRUCT(BlueprintType)
struct FRoundStatistics
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 ElapsedSeconds = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 LastCompletedRound = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 WaspsKilled = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 PollenCollected = 0;
};
