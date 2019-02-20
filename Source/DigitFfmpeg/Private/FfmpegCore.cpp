#include "FfmpegCore.h"

UDigitFfmpegCore::UDigitFfmpegCore()
{
}

UDigitFfmpegCore::~UDigitFfmpegCore()
{
}

UDigitFfmpegCore* UDigitFfmpegCore::CreateDigitFfmpeg(FString url)
{
	auto obj = NewObject<UDigitFfmpegCore>();
	obj->device_url = url;
	return obj;
}

bool UDigitFfmpegCore::Init()
{
	if(bInit)
	{
		return true;
	}

	avformat_network_init();

	if (avformat_open_input(&m_format_context, TCHAR_TO_ANSI(*device_url), nullptr, nullptr) < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not open file!"));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Yeeeee!"));

	return true;
}
