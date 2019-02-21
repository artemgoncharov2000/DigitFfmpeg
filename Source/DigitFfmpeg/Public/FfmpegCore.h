#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

extern "C" {
#include <ThirdParty/ffmpeg/include/libavcodec/avcodec.h>
#include <ThirdParty/ffmpeg/include/libswscale/swscale.h>
#include <ThirdParty/ffmpeg/include/libavformat/avformat.h>
}

#include "FfmpegCore.generated.h"


UCLASS(Blueprintable, BlueprintType)
class DIGITFFMPEG_API UDigitFfmpegCore : public UObject
{
	GENERATED_BODY()

public:

	UDigitFfmpegCore();
	~UDigitFfmpegCore();

	UFUNCTION(BlueprintCallable)
	static UDigitFfmpegCore* CreateDigitFfmpeg(FString url);

	UFUNCTION(BlueprintCallable)
		bool Init();

private:

	bool bInit = false;

	FString device_url;
	
	int m_video_index;

	AVCodec* m_decoder;
	AVCodecContext* m_decoder_context;
	AVFormatContext* m_format_context;
	
	AVFrame* m_frame;
	uint8_t m_buffer_frame_data;
	AVPacket m_packet;
};