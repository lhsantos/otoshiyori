#include "Otoshiyori.h"
#include "PositionServiceComponent.h"

DEFINE_LOG_CATEGORY(PositionService);



FPositionData::FPositionData(
		const struct FVector2D& InPosition /* = FVector2D::ZeroVector*/,
		const float InAltitude /* = 0.0f */,
		const float InAccuracy /* = 0.0f */)
	: Position(InPosition)
	, Altitude(InAltitude)
	, Accuracy(InAccuracy) {}


UPositionServiceComponent::UPositionServiceComponent() = default;

UPositionServiceComponent::~UPositionServiceComponent() = default;

bool UPositionServiceComponent::Start()
{
	return false;
}

bool UPositionServiceComponent::Stop()
{
	return false;
}
