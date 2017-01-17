#pragma once

#include "PositionServiceComponent.h"


class UPositionServiceComponentPlatform : public UPositionServiceComponent
{
public:
	UPositionServiceComponentPlatform();
	virtual ~UPositionServiceComponentPlatform();

	virtual bool Start() override;

	virtual bool Stop() override;

private:
	FDelegateHandle DelegateHandle;

	void FireLocationEvent(const FPositionData& LocationData);
};
