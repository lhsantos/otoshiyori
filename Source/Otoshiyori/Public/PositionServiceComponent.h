#pragma once

#include "Components/ActorComponent.h"
#include "PositionServiceComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PositionService, Log, All);


USTRUCT(BlueprintType)
struct FPositionData
{
	GENERATED_BODY()
	
	/** The global position (Latitude, Longitude). */
	UPROPERTY(BlueprintReadOnly)
	struct FVector2D Position;

	/** The altitude to sea level. */
	UPROPERTY(BlueprintReadOnly)
	float Altitude;

	/** The precision of the measurement. */
	UPROPERTY(BlueprintReadOnly)
	float Accuracy;

	FPositionData(const struct FVector2D& InPosition = FVector2D::ZeroVector, const float InAltitude = 0.0f, const float InAccuracy = 0.0f);
};

UCLASS(ClassGroup = (PositionService), meta = (BlueprintSpawnableComponent, ShortTooltip = "A (global) position service."))
class OTOSHIYORI_API UPositionServiceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FPositionServiceEventDelegate, const FPositionData&);
	FPositionServiceEventDelegate Listeners;

	UPROPERTY(BlueprintReadOnly)
	FPositionData LastPosition;

	UPositionServiceComponent();
	virtual ~UPositionServiceComponent();

	/**
	 * Starts the location service on the platform.
	 *
	 * @return - True, if successfully started the service.
	 */
	UFUNCTION(BlueprintCallable, Category = PositionService)
	virtual bool Start();

	/**
	 * Stops the location service on the platform.
	 *
	 * @return - True, if successfully stopped the service.
	 */
	UFUNCTION(BlueprintCallable, Category = PositionService)
	virtual bool Stop();
};
