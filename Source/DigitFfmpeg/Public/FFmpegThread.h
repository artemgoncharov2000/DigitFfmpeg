#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
#include "Engine/Texture2D.h"
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
	
	FFmpegThread(FString url, UTexture2D* texture);
	~FFmpegThread();

	bool Init() override;
	uint32 Run() override;
	void Exit() override;
	void Stop() override;

private:

	bool bWorkingThread = true;
	bool bInit = false;

	FString device_url;

	TArray<uint8> bufferTexture;

	AVCodec* m_decoder;
	AVCodecContext* m_decoder_context;
	AVFormatContext* m_format_context;
	AVInputFormat* device;

	AVFrame* m_frame;
	std::vector<uint8_t> m_buffer_frame_data;
	AVPacket m_packet{};
	SwsContext* sws_context;
	UTexture2D* TextureVideo;
	//IImageWrapper* ImgWrap;
};