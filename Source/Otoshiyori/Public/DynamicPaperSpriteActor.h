#pragma once

#include "PaperSpriteActor.h"
#include "DynamicPaperSprite.h"
#include "DynamicPaperSpriteActor.generated.h"


UCLASS()
class OTOSHIYORI_API ADynamicPaperSpriteActor : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = DynamicPaperSprite)
	void UpdateTexture(UTexture2D* Texture);
};
