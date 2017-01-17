#include "Otoshiyori.h"
#include "DynamicPaperSpriteActor.h"


void ADynamicPaperSpriteActor::UpdateTexture(UTexture2D* Texture)
{
	UPaperSpriteComponent* Render = Super::GetRenderComponent();
	Render->SetMobility(EComponentMobility::Stationary);
	UDynamicPaperSprite* Sprite = dynamic_cast<UDynamicPaperSprite*>(Render->GetSprite());
	if (!Sprite)
		Sprite = NewObject<UDynamicPaperSprite>();
	Sprite->SetTexture(Texture);
	Render->SetSprite(Sprite);
}
