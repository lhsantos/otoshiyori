#pragma once

#include "GameFramework/Actor.h"
#include "PositionServiceComponent.h"
#include "PositionServiceActor.generated.h"


UCLASS(Blueprintable, meta = (ShortTooltip = "Handles platform's location service API."))
class OTOSHIYORI_API APositionServiceActor : public AActor
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPositionChangedDelegate, const FPositionData&, Position);
	UPROPERTY(BlueprintAssignable)
	FPositionChangedDelegate OnPositionChanged;

	// Sets default values for this actor's properties
	APositionServiceActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	UPositionServiceComponent* PositionService;

	void OnPositionChangedInternal(const FPositionData& Data);
};
