#include "FFmpegThread.h"
#include "Runtime/Core/Public/HAL/PlatformProcess.h"
#include "Engine/Texture2DDynamic.h"

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
}

FFmpegThread::FFmpegThread(FString url, UTexture2D* texture) : device_url(url),
                                                               TextureVideo(texture),
                                                               m_decoder(nullptr),
                                                               m_decoder_context(nullptr),
                                                               m_format_context(nullptr), device(nullptr),
                                                               m_frame(nullptr),
                                                               m_buffer_frame_data(0), sws_context(nullptr)
{
}

FFmpegThread::~FFmpegThread()
{
	avformat_close_input(&m_format_context);
}

bool FFmpegThread::Init()
{
	av_register_all();
	avcodec_register_all();
	avdevice_register_all();
	return true;
}

uint32 FFmpegThread::Run()
{
	if (!bInit) {
		m_format_context = avformat_alloc_context();
		device = av_find_input_format("dshow");


		if (avformat_open_input(&m_format_context, TCHAR_TO_ANSI(*("video=" + device_url)), device, nullptr) < 0) {
			UE_LOG(LogTemp, Error, TEXT("Could not open input file"));
			return -1;
		}
		if (avformat_find_stream_info(m_format_context, nullptr) < 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to retrieve input stream information"));
			return -2;
		}

		UE_LOG(LogTemp, Log, TEXT("Successfully opened data stream %s"), *device_url);

		av_dump_format(m_format_context, 0, TCHAR_TO_ANSI(*("video=" + device_url)), 0);
		AVStream* m_stream = nullptr;

		unsigned int videoStream = 1;
		for (unsigned int i = 0; i < m_format_context->nb_streams; i++)
		{
			if (m_format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				m_stream = m_format_context->streams[i];
				break;
			}
		}

		if (m_stream->codecpar->codec_type != AVMEDIA_TYPE_VIDEO)
			return -3;


		m_decoder = avcodec_find_decoder(m_stream->codecpar->codec_id);
		if (!m_decoder)
			return -4;

		m_decoder_context = avcodec_alloc_context3(m_decoder);

		if (!m_decoder_context)
			return -5;

		if (avcodec_parameters_to_context(m_decoder_context, m_stream->codecpar) < 0) {
			return -6;
		}

		if (avcodec_open2(m_decoder_context, m_decoder, nullptr) < 0) {
			return -7;
		}

		if (m_decoder_context->time_base.num > 1000 && m_decoder_context->time_base.den == 1) {
			m_decoder_context->time_base.den = 1000;
		}

		const auto format = AV_PIX_FMT_RGBA;
		const FVector2D m_size(m_decoder_context->width, m_decoder_context->height);

		sws_context = sws_getContext(m_decoder_context->width, m_decoder_context->height, m_decoder_context->pix_fmt,
			m_size.X, m_size.Y, format,
			SWS_BICUBIC, nullptr, nullptr, nullptr);

		m_frame = av_frame_alloc();

		m_frame->width = m_size.X;
		m_frame->height = m_size.Y;
		m_frame->format = format;

		const auto size = av_image_get_buffer_size(format, m_frame->width, m_frame->height, 1);
		bufferTexture.AddUninitialized(size);

		if (size < 0)
			return -8;

		m_buffer_frame_data.resize(size);

		av_image_fill_arrays(m_frame->data, m_frame->linesize, m_buffer_frame_data.data(), format, m_frame->width, m_frame->height, 1);

		bInit = true;
	}

	while (bWorkingThread)
	{
		FPlatformProcess::Sleep(0.05f);

		if (!bInit)
			continue;

		if(av_read_frame(m_format_context, &m_packet) < 0)
		{
			continue;
		}

		if(avcodec_send_packet(m_decoder_context, &m_packet) < 0)
		{
			continue;
		}


		AVFrame* frame = av_frame_alloc();

		if (avcodec_receive_frame(m_decoder_context, frame) < 0)
		{
			UE_LOG(LogTemp, Log, TEXT("NOOOO"));
			av_frame_free(&frame);
			continue;
		}

		sws_scale(sws_context, frame->data, frame->linesize, 0, m_decoder_context->height, m_frame->data, m_frame->linesize);

		FMemory::Memcpy(bufferTexture.GetData(), m_frame->data[0], bufferTexture.Num());

		UE_LOG(LogTemp, Log, TEXT("%i"), bufferTexture.Num());

		FUpdateTextureRegion2D* test = new FUpdateTextureRegion2D();
		test->Height = static_cast<uint32>(frame->height);
		test->Width = static_cast<uint32>(frame->width);
		TextureVideo->UpdateTextureRegions(0, 0, test, frame->coded_picture_number, bufferTexture.Num(), bufferTexture.GetData());

		av_frame_free(&frame);
		av_packet_unref(&m_packet);


	}
	return 0;
}

void FFmpegThread::Exit()
{
}

void FFmpegThread::Stop()
{
	bWorkingThread = false;
}