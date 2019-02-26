#include "FFmpegThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
}

FFmpegThread::FFmpegThread(FString url) : m_video_index(0), 
										  m_decoder(nullptr), 
										  m_decoder_context(nullptr),
										  m_format_context(nullptr),
										  m_frame(nullptr),
										  m_buffer_frame_data(0),
										  device_url(url)
{

}

FFmpegThread::~FFmpegThread()
{
}

bool FFmpegThread::Init()
{
	return true;
}

uint32 FFmpegThread::Run()
{
	if (avformat_open_input(&m_format_context, TCHAR_TO_ANSI(*device_url), nullptr, nullptr) < 0) {
		UE_LOG(LogTemp, Error, TEXT("Could not open input file"));
		return -1;
	}
	if (avformat_find_stream_info(m_format_context, nullptr) < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve input stream information"));
		return -1;
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully opened data stream %s"), *device_url);

	while(bWorkingThread)
	{
		FPlatformProcess::Sleep(0.05f);


	}
	return 0;
}

void FFmpegThread::Exit()
{
	avformat_close_input(&m_format_context);
}

void FFmpegThread::Stop()
{
	bWorkingThread = false;
}
