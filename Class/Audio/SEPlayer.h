#pragma once
#include "../Adapter/Adapter.h"
#include <vector>
#include "AudioPlayer.h"

/// <summary>
/// SEの再生、停止を管理するクラス
/// </summary>
class SEPlayer
{
public:
	SEPlayer();
	~SEPlayer();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// SE再生
	/// </summary>
	/// <param name="fileName">読み込むファイルのパス</param>
	/// <param name="name">データの名前</param>
	/// <param name="volume">音量</param>
	void PlaySE(const std::string& fileName, const std::string& name, float volume);
	/// <summary>
	/// 指定したデータの音量をセット
	/// </summary>
	/// <param name="name">データの名前</param>
	/// <param name="volume">音量</param>
	void SetVolume(const std::string& name, float volume);
	/// <summary>
	/// 指定したデータの音を停止させる
	/// </summary>
	/// <param name="name">データの名前</param>
	void Stop(const std::string& name);
	/// <summary>
	/// 指定したデータの音が再生されているかどうかを返す
	/// </summary>
	/// <param name="name">データの名前</param>
	/// <returns></returns>
	bool GetIsPlaying(const std::string& name);

private:

	/// <summary>
	/// オーディオデータ取得
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	AudioPlayer* GetAudioPlayer(const std::string& name);

private:

	//オーディオデータの配列
	std::vector<AudioPlayer> audioPlayers_;

};


