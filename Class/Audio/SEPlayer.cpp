#include "SEPlayer.h"

SEPlayer::SEPlayer()
{
	audioPlayers_.clear();
}

SEPlayer::~SEPlayer()
{

	for (int32_t i = 0; i < audioPlayers_.size(); i++) {
		audioPlayers_[i].Stop();
	}

	audioPlayers_.clear();

}

void SEPlayer::Update() {

	//再生が終わったものは削除していく
	audioPlayers_.erase(std::remove_if(audioPlayers_.begin(), audioPlayers_.end(),
		[](AudioPlayer& audioPlayer) {

			if (not audioPlayer.GetIsPlaying()) {
				return true;
			}

			return false;

		}), audioPlayers_.end());

}

void SEPlayer::PlaySE(const std::string& fileName, const std::string& name, float volume)
{

	std::string filePath = "SE/" + fileName;

	AudioPlayer audioPlayer{};
	audioPlayers_.push_back(audioPlayer);
	audioPlayers_.back().Init(filePath, name, volume, 0);
	audioPlayers_.back().Play();

}

void SEPlayer::SetVolume(const std::string& name, float volume)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		audioPlayer->SetVolume(volume);
	}

}

void SEPlayer::Stop(const std::string& name)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		audioPlayer->Stop();
	}

}

bool SEPlayer::GetIsPlaying(const std::string& name)
{

	AudioPlayer* audioPlayer = GetAudioPlayer(name);

	if (audioPlayer) {
		return audioPlayer->GetIsPlaying();
	}

	return false;
}

AudioPlayer* SEPlayer::GetAudioPlayer(const std::string& name)
{

	for (int32_t i = 0; i < audioPlayers_.size(); i++) {
		//名前が一致しているものを返す
		if (audioPlayers_[i].GetName() == name) {
			return &audioPlayers_[i];
		}

	}

	return nullptr;

}

