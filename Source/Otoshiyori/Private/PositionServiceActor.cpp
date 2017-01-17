#include "Otoshiyori.h"
#include "PositionServiceActor.h"
#if PLATFORM_ANDROID
#include "PositionServiceComponentPlatform.h"
#endif


APositionServiceActor::APositionServiceActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, PositionService(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

#if PLATFORM_ANDROID
	PositionService = CreateDefaultSubobject<UPositionServiceComponentPlatform>(TEXT("PositionServiceComponent"));
#else
	PositionService = CreateDefaultSubobject<UPositionServiceComponent>(TEXT("PositionServiceComponent"));
#endif
}

void APositionServiceActor::BeginPlay()
{
	Super::BeginPlay();

	if (PositionService)
	{
		PositionService->Listeners.AddUObject(this, &APositionServiceActor::OnPositionChangedInternal);
		PositionService->Start();
	}
}

void APositionServiceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APositionServiceActor::OnPositionChangedInternal(const FPositionData& Data)
{
	OnPositionChanged.Broadcast(Data);
}
