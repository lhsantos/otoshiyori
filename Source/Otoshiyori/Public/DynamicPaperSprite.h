#pragma once

#include "PaperSprite.h"
#include "DynamicPaperSprite.generated.h"


UCLASS()
class OTOSHIYORI_API UDynamicPaperSprite : public UPaperSprite
{
	GENERATED_BODY()

public:
	
	UDynamicPaperSprite(const FObjectInitializer& ObjectInitializer);

	void SetTexture(UTexture2D* Texture);
};
