#include "Otoshiyori.h"
#include "GoogleMapsComponent.h"

DEFINE_LOG_CATEGORY(GoogleMaps);


UGoogleMapsComponent::UGoogleMapsComponent()
	: BaseURL(TEXT("https://maps.googleapis.com/maps/api/"))
	, Key(TEXT("AIzaSyDvhdoxUdBfX1290JkW0QrC0TCLLFcGnDE"))
	, Http(&FHttpModule::Get())
	, ImageWrapperModule(&FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper")))
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGoogleMapsComponent::PostLoad()
{
	Super::PostLoad();

	UE_LOG(GoogleMaps, Warning, TEXT("Map Size: %d,%d."), MapSize.X, MapSize.Y);
	MapTexture = UTexture2D::CreateTransient(MapSize.X, MapSize.Y, PF_B8G8R8A8);
}

bool UGoogleMapsComponent::RequestMap(const FVector2D& Center, const float MinRadius, FMapReceivedDelegate OnMapReceived)
{
	// Validates the output texture.
	if (!MapTexture)
	{
		UE_LOG(GoogleMaps, Warning, TEXT("Invalid map texture."));
		return false;
	}

	FString URL = BaseURL + TEXT("staticmap?key=") + Key;
	URL.Append(TEXT("&center="));
	URL.Append(FString::SanitizeFloat(Center.X));
	URL.Append(TEXT(","));
	URL.Append(FString::SanitizeFloat(Center.Y));
	URL.Append(TEXT("&zoom=14&size=512x512&format=png"));

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->OnProcessRequestComplete().BindLambda([this, OnMapReceived](FHttpRequestPtr ProcessedRequest, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		UE_LOG(GoogleMaps, Log, TEXT("Handling response from %s request to %s"), *ProcessedRequest->GetVerb(), *ProcessedRequest->GetURL());

		// Validates request status...
		if (EHttpRequestStatus::Succeeded != ProcessedRequest->GetStatus())
		{
			const int32 Status = ProcessedRequest->GetStatus();
			const FString StatusDesc = EHttpRequestStatus::ToString(ProcessedRequest->GetStatus());
			UE_LOG(GoogleMaps, Warning, TEXT("Request failed with status: %d - %s."), Status, *StatusDesc);
			return;
		}
		// Validates response code...
		if (EHttpResponseCodes::Ok != Response->GetResponseCode())
		{
			UE_LOG(GoogleMaps, Warning, TEXT("Response invalid with status code: %d."), Response->GetResponseCode());
			return;
		}
		// Validates content...
		const FString ContentType = Response->GetContentType();
		if (!ContentType.Contains(TEXT("png")))
		{
			UE_LOG(GoogleMaps, Warning, TEXT("Response with unexpected content type: %s."), *ContentType);
			return;
		}

		IImageWrapperPtr ImageWrapper = ImageWrapperModule->CreateImageWrapper(EImageFormat::PNG);
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Response->GetContent().GetData(), Response->GetContentLength()))
		{
			const TArray<uint8>* UncompressedBGRA = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				// Fill in the source data from the file.
				FTexture2DMipMap& Mip = MapTexture->PlatformData->Mips[0];
				void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
				Mip.BulkData.Unlock();
				MapTexture->UpdateResource();
				OnMapReceived.ExecuteIfBound(MapTexture);
				UE_LOG(GoogleMaps, Log, TEXT("Loaded map successfully. Size: %dx%d."), MapTexture->PlatformData->SizeX, MapTexture->PlatformData->SizeY);
			}
			else
			{
				UE_LOG(GoogleMaps, Warning, TEXT("Failed to decompress texture data."));
			}
		}
	});

	return Request->ProcessRequest();
}
