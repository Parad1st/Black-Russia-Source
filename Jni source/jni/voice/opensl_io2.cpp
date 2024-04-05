/*
  opensl_io.c:
  Android OpenSL input/output module
  Copyright (c) 2012, Victor Lazzarini
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.
  * Neither the name of the <organization> nor the
  names of its contributors may be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "opensl_io2.h"
#define CONV16BIT 32768
#define CONVMYFLT (1./32768.)
#define GRAIN 4

static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context);
static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void* context);

// creates the OpenSL ES audio engine
static SLresult openSLCreateEngine(OPENSL_STREAM* p)
{
	SLresult result;
	// create engine
	result = slCreateEngine(&(p->engineObject), 0, NULL, 0, NULL, NULL);
	if (result != SL_RESULT_SUCCESS) goto  engine_end;

	// realize the engine 
	result = (*p->engineObject)->Realize(p->engineObject, SL_BOOLEAN_FALSE);
	if (result != SL_RESULT_SUCCESS) goto engine_end;

	// get the engine interface, which is needed in order to create other objects
	result = (*p->engineObject)->GetInterface(p->engineObject, SL_IID_ENGINE, &(p->engineEngine));
	if (result != SL_RESULT_SUCCESS) goto  engine_end;

engine_end:
	return result;
}

// opens the OpenSL ES device for output
static SLresult openSLPlayOpen(OPENSL_STREAM* p)
{
	SLresult result;
	SLuint32 sr = p->sr;
	SLuint32  channels = p->outchannels;

	if (channels) {
		// configure audio source
		SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };

		switch (sr) {

		case 8000:
			sr = SL_SAMPLINGRATE_8;
			break;
		case 11025:
			sr = SL_SAMPLINGRATE_11_025;
			break;
		case 16000:
			sr = SL_SAMPLINGRATE_16;
			break;
		case 22050:
			sr = SL_SAMPLINGRATE_22_05;
			break;
		case 24000:
			sr = SL_SAMPLINGRATE_24;
			break;
		case 32000:
			sr = SL_SAMPLINGRATE_32;
			break;
		case 44100:
			sr = SL_SAMPLINGRATE_44_1;
			break;
		case 48000:
			sr = SL_SAMPLINGRATE_48;
			break;
		case 64000:
			sr = SL_SAMPLINGRATE_64;
			break;
		case 88200:
			sr = SL_SAMPLINGRATE_88_2;
			break;
		case 96000:
			sr = SL_SAMPLINGRATE_96;
			break;
		case 192000:
			sr = SL_SAMPLINGRATE_192;
			break;
		default:
			return -1;
		}

		const SLInterfaceID ids[] = { SL_IID_VOLUME };
		const SLboolean req[] = { SL_BOOLEAN_FALSE };
		result = (*p->engineEngine)->CreateOutputMix(p->engineEngine, &(p->outputMixObject), 1, ids, req);
		if (result != SL_RESULT_SUCCESS)
		{
			return result;
		}

		// realize the output mix
		result = (*p->outputMixObject)->Realize(p->outputMixObject, SL_BOOLEAN_FALSE);

		//result = android_CreatePlayer(p);

	end_openaudio:
		return result;
	}
	return SL_RESULT_SUCCESS;
}

// Open the OpenSL ES device for input
static SLresult openSLRecOpen(OPENSL_STREAM* p) {

	SLresult result;
	SLuint32 sr = p->sr;
	SLuint32 channels = p->inchannels;

	if (channels) {

		switch (sr) {

		case 8000:
			sr = SL_SAMPLINGRATE_8;
			break;
		case 11025:
			sr = SL_SAMPLINGRATE_11_025;
			break;
		case 16000:
			sr = SL_SAMPLINGRATE_16;
			break;
		case 22050:
			sr = SL_SAMPLINGRATE_22_05;
			break;
		case 24000:
			sr = SL_SAMPLINGRATE_24;
			break;
		case 32000:
			sr = SL_SAMPLINGRATE_32;
			break;
		case 44100:
			sr = SL_SAMPLINGRATE_44_1;
			break;
		case 48000:
			sr = SL_SAMPLINGRATE_48;
			break;
		case 64000:
			sr = SL_SAMPLINGRATE_64;
			break;
		case 88200:
			sr = SL_SAMPLINGRATE_88_2;
			break;
		case 96000:
			sr = SL_SAMPLINGRATE_96;
			break;
		case 192000:
			sr = SL_SAMPLINGRATE_192;
			break;
		default:
			return -1;
		}

		// configure audio source
		SLDataLocator_IODevice loc_dev = { SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,
						  SL_DEFAULTDEVICEID_AUDIOINPUT, NULL };
		SLDataSource audioSrc = { &loc_dev, NULL };

		// configure audio sink
		SLuint32 speakers;
		if (channels > 1)
			speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		else speakers = SL_SPEAKER_FRONT_CENTER;
		SLDataLocator_AndroidSimpleBufferQueue loc_bq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
		SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM, channels, sr,
					   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
					   speakers, SL_BYTEORDER_LITTLEENDIAN };
		SLDataSink audioSnk = { &loc_bq, &format_pcm };

		// create audio recorder
		// (requires the RECORD_AUDIO permission)
		const SLInterfaceID id[2] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION };
		const SLboolean req[2] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
		result = (*p->engineEngine)->CreateAudioRecorder(p->engineEngine, &(p->recorderObject), &audioSrc,
			&audioSnk, 2, id, req);
		if (SL_RESULT_SUCCESS != result)
		{
			return result;
		}

		SLAndroidConfigurationItf recorder_config;
		
		result = (*p->recorderObject)->GetInterface(p->recorderObject, SL_IID_ANDROIDCONFIGURATION, &recorder_config);
		if (SL_RESULT_SUCCESS != result)
		{
			return result;
		}

		// Uses the default microphone tuned for audio communication.
		// Note that, SL_ANDROID_RECORDING_PRESET_VOICE_RECOGNITION leads to a fast
		// track but also excludes usage of required effects like AEC, AGC and NS.
		// SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION
		SLint32 stream_type = SL_ANDROID_RECORDING_PRESET_VOICE_COMMUNICATION;
		(*recorder_config)->SetConfiguration(recorder_config, SL_ANDROID_KEY_RECORDING_PRESET, &stream_type, sizeof(SLint32));

		// realize the audio recorder
		result = (*p->recorderObject)->Realize(p->recorderObject, SL_BOOLEAN_FALSE);
		if (SL_RESULT_SUCCESS != result) goto end_recopen;

		// get the record interface
		result = (*p->recorderObject)->GetInterface(p->recorderObject, SL_IID_RECORD, &(p->recorderRecord));
		if (SL_RESULT_SUCCESS != result) goto end_recopen;

		// get the buffer queue interface
		result = (*p->recorderObject)->GetInterface(p->recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
			&(p->recorderBufferQueue));
		if (SL_RESULT_SUCCESS != result) goto end_recopen;

		// register callback on the buffer queue
		result = (*p->recorderBufferQueue)->RegisterCallback(p->recorderBufferQueue, bqRecorderCallback,
			p);
		if (SL_RESULT_SUCCESS != result) goto end_recopen;

		if ((p->recBuffer = (short*)calloc(p->inBufSamples, sizeof(short))) == NULL) {
			return -1;
		}
		/*memset(p->recBuffer, 0, p->inBufSamples * sizeof(short));*/
		(*p->recorderBufferQueue)->Enqueue(p->recorderBufferQueue,
			p->recBuffer, p->inBufSamples * sizeof(short));

	end_recopen:
		return result;
	}
	else return SL_RESULT_SUCCESS;


}

// close the OpenSL IO and destroy the audio engine
static void openSLDestroyEngine(OPENSL_STREAM* p) {

	// destroy buffer queue audio player object, and invalidate all associated interfaces
	if (p->bqPlayerObject != NULL && p->bqPlayerPlay != NULL) {
		SLuint32 state = SL_PLAYSTATE_PLAYING;
		(*p->bqPlayerPlay)->SetPlayState(p->bqPlayerPlay, SL_PLAYSTATE_STOPPED);
		while (state != SL_PLAYSTATE_STOPPED)
			(*p->bqPlayerPlay)->GetPlayState(p->bqPlayerPlay, &state);
		(*p->bqPlayerObject)->Destroy(p->bqPlayerObject);
		p->bqPlayerObject = NULL;
		p->bqPlayerPlay = NULL;
		p->bqPlayerBufferQueue = NULL;
		p->bqPlayerEffectSend = NULL;
	}

	// destroy audio recorder object, and invalidate all associated interfaces
	if (p->recorderObject != NULL && p->recorderRecord != NULL)
	{
		SLuint32 state = SL_PLAYSTATE_PLAYING;
		(*p->recorderRecord)->SetRecordState(p->recorderRecord, SL_RECORDSTATE_STOPPED);
		while (state != SL_RECORDSTATE_STOPPED)
			(*p->recorderRecord)->GetRecordState(p->recorderRecord, &state);
		(*p->recorderObject)->Destroy(p->recorderObject);
		p->recorderObject = NULL;
		p->recorderRecord = NULL;
		p->recorderBufferQueue = NULL;
	}

	// destroy output mix object, and invalidate all associated interfaces
	if (p->outputMixObject != NULL) {
		(*p->outputMixObject)->Destroy(p->outputMixObject);
		p->outputMixObject = NULL;
	}

	// destroy engine object, and invalidate all associated interfaces
	if (p->engineObject != NULL) {
		(*p->engineObject)->Destroy(p->engineObject);
		p->engineObject = NULL;
		p->engineEngine = NULL;
	}

}


OPENSL_STREAM* android_copySlStream(OPENSL_STREAM* p)
{
	OPENSL_STREAM* newSL;
	newSL = (OPENSL_STREAM*)malloc(sizeof(OPENSL_STREAM));
	memcpy(newSL, p, sizeof(OPENSL_STREAM));

	newSL->context = newSL;

	newSL->outrb = new RingBuffer<char, 20000>();
	android_CreatePlayer(newSL);
	newSL->lastTimeInsertAudio = 0;
	newSL->time = 0.0;
	return newSL;
}

void android_destroyCopiedSlStream(OPENSL_STREAM* p)
{
	delete p->outrb;
	free((void*)p);
}

// open the android audio device for input and/or output
OPENSL_STREAM* android_OpenAudioDevice(int sr, int inchannels, int outchannels, int bufferframes, RecordCallback callback, void* context) {

	OPENSL_STREAM* p;
	p = (OPENSL_STREAM*)malloc(sizeof(OPENSL_STREAM));
	memset(p, 0, sizeof(OPENSL_STREAM));
	p->inchannels = inchannels;
	p->outchannels = outchannels;
	p->sr = sr;

	p->cb = callback;
	p->context = context;

	if ((p->outBufSamples = bufferframes * outchannels) != 0) {
		if ((p->outputBuffer = (short*)calloc(p->outBufSamples, sizeof(short))) == NULL) {
			android_CloseAudioDevice(p);
			return NULL;
		}
	}

	if ((p->inBufSamples = bufferframes * inchannels) != 0) {
		if ((p->inputBuffer = (short*)calloc(p->inBufSamples, sizeof(short))) == NULL) {
			android_CloseAudioDevice(p);
			return NULL;
		}
	}
	p->outrb = new RingBuffer<char, 20000>();
	if (p->outrb  == NULL) {
		android_CloseAudioDevice(p);
		return NULL;
	}
	p->inrb = new RingBuffer<char, 20000>();
	if (p->inrb == NULL) {
		android_CloseAudioDevice(p);
		return NULL;
	}

	if (openSLCreateEngine(p) != SL_RESULT_SUCCESS) {
		android_CloseAudioDevice(p);
		return NULL;
	}

	if (openSLRecOpen(p) != SL_RESULT_SUCCESS) {
		android_CloseAudioDevice(p);
		return NULL;
	}

	if (openSLPlayOpen(p) != SL_RESULT_SUCCESS) {
		android_CloseAudioDevice(p);
		return NULL;
	}

	p->time = 0.;
	return p;
}

SLresult android_CreatePlayer(OPENSL_STREAM* p)
{
	SLresult result;
	SLuint32 sr = p->sr;
	SLuint32  channels = p->outchannels;

	if (channels) {
		// configure audio source
		SLDataLocator_AndroidSimpleBufferQueue loc_bufq = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };

		switch (sr) {

		case 8000:
			sr = SL_SAMPLINGRATE_8;
			break;
		case 11025:
			sr = SL_SAMPLINGRATE_11_025;
			break;
		case 16000:
			sr = SL_SAMPLINGRATE_16;
			break;
		case 22050:
			sr = SL_SAMPLINGRATE_22_05;
			break;
		case 24000:
			sr = SL_SAMPLINGRATE_24;
			break;
		case 32000:
			sr = SL_SAMPLINGRATE_32;
			break;
		case 44100:
			sr = SL_SAMPLINGRATE_44_1;
			break;
		case 48000:
			sr = SL_SAMPLINGRATE_48;
			break;
		case 64000:
			sr = SL_SAMPLINGRATE_64;
			break;
		case 88200:
			sr = SL_SAMPLINGRATE_88_2;
			break;
		case 96000:
			sr = SL_SAMPLINGRATE_96;
			break;
		case 192000:
			sr = SL_SAMPLINGRATE_192;
			break;
		default:
			return -1;
		}

		SLuint32 speakers;
		if (channels > 1)
			speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
		else speakers = SL_SPEAKER_FRONT_CENTER;
		SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM,channels, sr,
					   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
					   speakers, SL_BYTEORDER_LITTLEENDIAN };

		SLDataSource audioSrc = { &loc_bufq, &format_pcm };

		// configure audio sink
		SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, p->outputMixObject };
		SLDataSink audioSnk = { &loc_outmix, NULL };

		// create audio player
		const SLInterfaceID ids1[] = { SL_IID_ANDROIDCONFIGURATION, SL_IID_ANDROIDSIMPLEBUFFERQUEUE };
		const SLboolean req1[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
		result = (*p->engineEngine)->CreateAudioPlayer(p->engineEngine, &(p->bqPlayerObject), &audioSrc, &audioSnk,
			2, ids1, req1);
		if (result != SL_RESULT_SUCCESS)
		{
			return result;
		}

		SLAndroidConfigurationItf player_config;
		// get the play interface
		result = (*p->bqPlayerObject)->GetInterface(p->bqPlayerObject, SL_IID_ANDROIDCONFIGURATION, &player_config);
		if (result != SL_RESULT_SUCCESS)
		{
			return result;
		}

		SLint32 stream_type = SL_ANDROID_STREAM_VOICE;
		(*player_config)->SetConfiguration(player_config, SL_ANDROID_KEY_STREAM_TYPE, &stream_type, sizeof(SLint32));

		// realize the player
		result = (*p->bqPlayerObject)->Realize(p->bqPlayerObject, SL_BOOLEAN_FALSE);
		if (result != SL_RESULT_SUCCESS) goto end_openaudio;

		// get the play interface
		result = (*p->bqPlayerObject)->GetInterface(p->bqPlayerObject, SL_IID_PLAY, &(p->bqPlayerPlay));
		if (result != SL_RESULT_SUCCESS) goto end_openaudio;

		// get the buffer queue interface
		result = (*p->bqPlayerObject)->GetInterface(p->bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
			&(p->bqPlayerBufferQueue));
		if (result != SL_RESULT_SUCCESS) goto end_openaudio;

		// register callback on the buffer queue
		result = (*p->bqPlayerBufferQueue)->RegisterCallback(p->bqPlayerBufferQueue, bqPlayerCallback, p);
		if (result != SL_RESULT_SUCCESS) goto end_openaudio;

		// set the player's state to playing
		result = (*p->bqPlayerPlay)->SetPlayState(p->bqPlayerPlay, SL_PLAYSTATE_PLAYING);

		if ((p->playBuffer = (short*)calloc(p->outBufSamples, sizeof(short))) == NULL) {
			return -1;
		}
		memset(p->playBuffer, 0, p->outBufSamples * sizeof(short));
		(*p->bqPlayerBufferQueue)->Enqueue(p->bqPlayerBufferQueue,
			p->playBuffer, p->outBufSamples * sizeof(short));

	end_openaudio:
		return result;
	}
	return SL_RESULT_SUCCESS;
}

// close the android audio device
void android_CloseAudioDevice(OPENSL_STREAM* p) {

	if (p == NULL)
		return;

	openSLDestroyEngine(p);

	if (p->outputBuffer != NULL) {
		free(p->outputBuffer);
		p->outputBuffer = NULL;
	}

	if (p->inputBuffer != NULL) {
		free(p->inputBuffer);
		p->inputBuffer = NULL;
	}

	if (p->playBuffer != NULL) {
		free(p->playBuffer);
		p->playBuffer = NULL;
	}

	if (p->recBuffer != NULL) {
		free(p->recBuffer);
		p->recBuffer = NULL;
	}

	delete p->inrb;
	delete p->outrb;

	free(p);
}

// returns timestamp of the processed stream
double android_GetTimestamp(OPENSL_STREAM* p) {
	return p->time;
}


// this callback handler is called every time a buffer finishes recording
void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void* context)
{
	OPENSL_STREAM* p = (OPENSL_STREAM*)context;
	int bytes = p->inBufSamples * sizeof(short);

	if (p->cb)
	{
		p->cb((void*)p->context, (void*)p->recBuffer, p->inBufSamples);
	}

	(*p->recorderBufferQueue)->Enqueue(p->recorderBufferQueue, p->recBuffer, bytes);
}
void Log(const char* fmt, ...);
uint32_t GetTickCount();
// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context)
{
	OPENSL_STREAM* p = (OPENSL_STREAM*)context;
	int bytes = p->outBufSamples * sizeof(short);
	if (GetTickCount() - p->lastTimeInsertAudio >= 200)
	{
		memset((void*)p->playBuffer, 0, bytes);
	}
	else
	{
		if (p->outrb->BytesToRead() < bytes)
		{
			memset((void*)p->playBuffer, 0, bytes);
		}
		else
		{
			p->outrb->Read((char*)p->playBuffer, bytes);
		}
	}
	(*p->bqPlayerBufferQueue)->Enqueue(p->bqPlayerBufferQueue, p->playBuffer, bytes);
}

// puts a buffer of size samples to the device
int android_AudioOut(OPENSL_STREAM* p, short* buffer, int size) {

	short* outBuffer, * inBuffer;
	int i, bytes = size * sizeof(short);
	if (p == NULL || p->outBufSamples == 0)  return 0;

	p->outrb->Write((char*)buffer, bytes);
	p->lastTimeInsertAudio = GetTickCount();
	p->time += (double)size / (p->sr * p->outchannels);
	return bytes / sizeof(short);
}