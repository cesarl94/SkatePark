// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "GM_SkatePark.generated.h"


/**
 *
 */
UCLASS()
class SKATEPARK_API AGM_SkatePark : public AGameModeBase {
	GENERATED_BODY()

private:
	int32 CollectablesCount;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetCollectablesCount() const;
};
