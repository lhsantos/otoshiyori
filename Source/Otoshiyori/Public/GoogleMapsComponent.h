#pragma once

#include "Components/ActorComponent.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Runtime/ImageWrapper/Public/ImageWrapper.h"
#include "GoogleMapsComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GoogleMaps, Log, All);


UCLASS( ClassGroup=(GoogleMaps), meta=(BlueprintSpawnableComponent, ShortTooltip = "Handles Google Maps API."))
class OTOSHIYORI_API UGoogleMapsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Called when a new map texture has been received from Goodle.
	DECLARE_DYNAMIC_DELEGATE_OneParam(FMapReceivedDelegate, UTexture2D*, MapTexture);

	// The base URL for accessing Google Maps.
	UPROPERTY(EditAnywhere)
	FString BaseURL;

	// The API key for accessing Google Maps.
	UPROPERTY(EditAnywhere)
	FString Key;

	UPROPERTY(EditDefaultsOnly)
	FIntPoint MapSize;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* MapTexture;

	// Sets default values for this component's properties
	UGoogleMapsComponent();

	// UObject interface
	virtual void PostLoad() override;
	// End of UObject interface

	/**
	 * Requests a new map texture from Google.
	 *
	 * @param Center Coordinates (Latitude, Longitude) of the center of the region to be shown in the map.
	 * @param MinRadius Minimum radius, in meters the map should show around the center point.
	 * @param OutTexture The target texture to be updated.
	 * @param OnMapReceived Will be called (if and) when the map is received from Google.
	 *
	 * @return True if the request was successful; false (and makes a log entry) otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = GoogleMaps)
	bool RequestMap(const FVector2D& Center, const float MinRadius, FMapReceivedDelegate OnMapReceived);

private:
	FHttpModule* Http;
	IImageWrapperModule* ImageWrapperModule;
};
