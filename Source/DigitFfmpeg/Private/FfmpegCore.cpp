#include "FfmpegCore.h"

UDigitFfmpegCore::UDigitFfmpegCore()
{
	
}

UDigitFfmpegCore::~UDigitFfmpegCore()
{
	if (Thread)
		Thread->Kill();
}

UDigitFfmpegCore* UDigitFfmpegCore::CreateDigitFfmpeg(FString url)
{
	auto obj = NewObject<UDigitFfmpegCore>();
	obj->ffmpeg_thread = MakeShared<FFmpegThread>(url);
	obj->Thread = FRunnableThread::Create(obj->ffmpeg_thread.Get(), TEXT("FFmpeg"));
	return obj;
}


