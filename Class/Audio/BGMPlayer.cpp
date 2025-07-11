#include "BGMPlayer.h"

BGMPlayer::BGMPlayer()
{
	audioPlayers_.clear();
}

BGMPlayer::~BGMPlayer()
{

	for (int32_t i = 0; i < audioPlayers_.size(); i++) {
		audioPlayers_[i].Stop();
	}

	audioPlayers_.clear();

}

void BGMPlayer::Update() {

	//再生が終わったものは削除していく
	audioPlayers_.erase(std::remove_if(audioPlayers_.begin(), audioPlayers_.end(),
		[](AudioPlayer& audioPlayer) {

			if (not audioPlayer.GetIsPlaying()) {
				return true;
			}

			return false;

		}), audioPlayers_.end());

}

void BGMPlayer::PlayBGM(const std::string& fileName, const std::string& name, float volume)
{

	std::string filePath = "BGM/" + fileName;

	AudioPlayer audioPlayer{};
	audioPlayers_.push_back(audioPlayer);
	audioPlayers_.back().Init(filePath, name, volume, 255);
	audioPlayers_.back().Play();

}

void BGMPlayer::SetVolume(const std::string& name, float volume)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		audioPlayer->SetVolume(volume);
	}

}

void BGMPlayer::Stop(const std::string& name)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		audioPlayer->Stop();
	}

}

bool BGMPlayer::GetIsPlaying(const std::string& name)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		return audioPlayer->GetIsPlaying();
	}

	return false;
}

AudioPlayer* BGMPlayer::GetAudioPlayer(const std::string& name)
{
	
	for (int32_t i = 0; i < audioPlayers_.size(); i++) {
		//名前が一致しているものを返す
		if (audioPlayers_[i].GetName() == name) {
			return &audioPlayers_[i];
		}

	}

	return nullptr;

}
