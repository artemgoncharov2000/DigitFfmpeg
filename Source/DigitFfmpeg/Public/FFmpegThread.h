#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include <vector>

extern "C" {
#include <ThirdParty/ffmpeg/include/libavcodec/avcodec.h>
#include <ThirdParty/ffmpeg/include/libswscale/swscale.h>
#include <ThirdParty/ffmpeg/include/libavformat/avformat.h>
#include <ThirdParty/ffmpeg/include/libavdevice/avdevice.h>
}

class FFmpegThread : public FRunnable
{
public:
	
	FFmpegThread(FString url);
	~FFmpegThread();

	bool Init() override;
	uint32 Run() override;
	void Exit() override;
	void Stop() override;

	bool bWorkingThread = true;
private:
	
	bool bInit = false;


	FString device_url;

	int m_video_index;

	AVCodec* m_decoder;
	AVCodecContext* m_decoder_context;
	AVFormatContext* m_format_context;
	AVInputFormat* device;

	AVFrame* m_frame;
	std::vector<uint8_t> m_buffer_frame_data;
	AVPacket m_packet{};
};