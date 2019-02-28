#include "FfmpegCore.h"

UDigitFfmpegCore::UDigitFfmpegCore()
{
	
}

UDigitFfmpegCore::~UDigitFfmpegCore()
{
	if (Thread) {
		Thread->Kill();
		Thread->WaitForCompletion();
	}
}

UDigitFfmpegCore* UDigitFfmpegCore::CreateDigitFfmpeg(FString url)
{
	auto obj = NewObject<UDigitFfmpegCore>();
	obj->Texture = NewObject<UTexture2D>();
	obj->ffmpeg_thread = MakeShared<FFmpegThread>(url, obj->Texture);
	obj->Thread = FRunnableThread::Create(obj->ffmpeg_thread.Get(), TEXT("FFmpeg"));
	return obj;
}

UTexture2D* UDigitFfmpegCore::GetTexture()
{
	return Texture;
}


