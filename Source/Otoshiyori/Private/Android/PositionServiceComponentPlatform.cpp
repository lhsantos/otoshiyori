#include "Otoshiyori.h"
#include "PositionServiceComponentPlatform.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"


extern "C"
{
	void Java_com_epicgames_ue4_GameActivity_Otoshiyori_1LocationChanged(JNIEnv*, jobject, jobject);
}

class FAndroidLocationService
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FAndroidLocationEventDelegate, const FPositionData&);
	FAndroidLocationEventDelegate Delegate;

	static FAndroidLocationService& Get()
	{
		static FAndroidLocationService Instance;
		return Instance;
	}


private:
	friend void Java_com_epicgames_ue4_GameActivity_Otoshiyori_1LocationChanged(JNIEnv*, jobject, jobject);

	// JNI
	static jclass LocationClass;
	static jmethodID GetLatitudeMethod;
	static jmethodID GetLongitudeMethod;
	static jmethodID GetAltitudeMethod;
	static jmethodID GetAccuracyMethod;

	FAndroidLocationService()
	{
		JNIEnv* Env = FAndroidApplication::GetJavaEnv();
		LocationClass = FJavaWrapper::FindClass(Env, "android/location/Location", false);
		GetLatitudeMethod = FJavaWrapper::FindMethod(Env, LocationClass, "getLatitude", "()D", false);
		GetLongitudeMethod = FJavaWrapper::FindMethod(Env, LocationClass, "getLongitude", "()D", false);
		GetAltitudeMethod = FJavaWrapper::FindMethod(Env, LocationClass, "getAltitude", "()D", false);
		GetAccuracyMethod = FJavaWrapper::FindMethod(Env, LocationClass, "getAccuracy", "()F", false);
	}

	void OnLocationChanged(JNIEnv* Env, jobject LocationJavaObj)
	{
		FPositionData Data;
		Data.Position.X = (float)Env->CallDoubleMethod(LocationJavaObj, GetLatitudeMethod);
		Data.Position.Y = (float)Env->CallDoubleMethod(LocationJavaObj, GetLongitudeMethod);
		Data.Altitude = (float)Env->CallDoubleMethod(LocationJavaObj, GetAltitudeMethod);
		Data.Accuracy = (float)Env->CallFloatMethod(LocationJavaObj, GetAccuracyMethod);

		Delegate.Broadcast(Data);
	}

	FAndroidLocationService(const FAndroidLocationService&) = delete;
	FAndroidLocationService(FAndroidLocationService&&) = delete;
	FAndroidLocationService& operator=(const FAndroidLocationService&) = delete;
	FAndroidLocationService& operator=(FAndroidLocationService&&) = delete;
	~FAndroidLocationService() = default;
};

jclass FAndroidLocationService::LocationClass;
jmethodID FAndroidLocationService::GetLatitudeMethod;
jmethodID FAndroidLocationService::GetLongitudeMethod;
jmethodID FAndroidLocationService::GetAltitudeMethod;
jmethodID FAndroidLocationService::GetAccuracyMethod;


void Java_com_epicgames_ue4_GameActivity_Otoshiyori_1LocationChanged(JNIEnv* jenv, jobject thiz, jobject location)
{
	FAndroidLocationService::Get().OnLocationChanged(jenv, location);
}

UPositionServiceComponentPlatform::UPositionServiceComponentPlatform() = default;

UPositionServiceComponentPlatform::~UPositionServiceComponentPlatform() = default;

bool UPositionServiceComponentPlatform::Start()
{
	if (!DelegateHandle.IsValid())
	{
		DelegateHandle = FAndroidLocationService::Get().Delegate.AddUObject(this, &UPositionServiceComponentPlatform::FireLocationEvent);
		return true;
	}
	return false;
}

bool UPositionServiceComponentPlatform::Stop()
{
	if (DelegateHandle.IsValid())
	{
		FAndroidLocationService::Get().Delegate.Remove(DelegateHandle);
		return true;
	}
	return false;
}

void UPositionServiceComponentPlatform::FireLocationEvent(const FPositionData& LocationData)
{
	Listeners.Broadcast(LocationData);
}
