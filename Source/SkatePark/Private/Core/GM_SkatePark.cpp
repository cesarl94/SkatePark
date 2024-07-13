#include "Core/GM_SkatePark.h"

#include "Actors/Collectable.h"
#include "Kismet/GameplayStatics.h"


void AGM_SkatePark::BeginPlay() {
	Super::BeginPlay();

	TArray<AActor *> Collectables;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectable::StaticClass(), Collectables);
	CollectablesCount = Collectables.Num();
}

int32 AGM_SkatePark::GetCollectablesCount() const { return CollectablesCount; }