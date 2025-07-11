#include "AudioPlayer.h"

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::Init(const std::string& fileName, const std::string& name, float volume, UINT loopCount)
{

	audio_.LoadShortPath(fileName);
	name_ = name;
	volume_ = volume;
	loopCount_ = loopCount;

}

void AudioPlayer::Play() {

	audio_.PlayAnyLoop(loopCount_);
	audio_.SetVolume(volume_);

}

void AudioPlayer::Stop() {

	audio_.Stop();

}

void AudioPlayer::SetVolume(float volume)
{
	volume_ = volume;
	audio_.SetVolume(volume_);

}
