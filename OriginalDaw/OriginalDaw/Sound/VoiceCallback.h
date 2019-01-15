#pragma once
#include <xaudio2.h>

class VoiceCallback :
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	VoiceCallback() : handle(CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS)) {
	}
	// デストラクタ
	~VoiceCallback() {
		CloseHandle(handle);
	}

	void STDMETHODCALLTYPE OnStreamEnd() {
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {
	}
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(unsigned int SamplesRequired) {
	}
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext) {
		SetEvent(handle);
	}
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {
	}
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) {
	}
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, long Error) {
	}

	// ハンドル
	void* handle;
};

