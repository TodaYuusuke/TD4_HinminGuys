#pragma once
#include "../Adapter/Adapter.h"
#include <string>

class AudioPlayer
{
public:
	AudioPlayer();
	~AudioPlayer();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="fileName">ロードするファイルの名前</param>
	/// <param name="name">データの名前</param>
	/// <param name="volume">音量</param>
	/// <param name="loopCount">ループカウント</param>
	void Init(const std::string& fileName, const std::string& name, float volume, UINT loopCount);

	/// <summary>
	/// 再生
	/// </summary>
	void Play();
	/// <summary>
	/// 停止
	/// </summary>
	void Stop();
	/// <summary>
	/// 音量セット
	/// </summary>
	/// <param name="volume">音量</param>
	void SetVolume(float volume);

	/// <summary>
	/// 名前取得
	/// </summary>
	/// <returns>オーディオの名前</returns>
	const std::string& GetName() const { return name_; }
	/// <summary>
	/// 再生中かどうかを取得
	/// </summary>
	/// <returns>再生中かどうか</returns>
	bool GetIsPlaying() { return audio_.GetIsPlaying(); }

private:

	std::string name_;

	LWP::Resource::Audio audio_;

	float volume_ = 1.0f;
	UINT loopCount_ = 0;

};


