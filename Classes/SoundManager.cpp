#include "SoundManager.h"
#include "AudioEngine.h"

SoundManager* SoundManager::m_instance = nullptr;

using namespace cocos2d::experimental;

SoundManager::SoundManager() {
	m_soundMap.clear();
}

SoundManager * SoundManager::getInstance()
{
	if (!m_instance)
		m_instance = new SoundManager();
	return m_instance;
}

void SoundManager::playSound(std::string path, bool loop, std::function<void()> finishCallback /*= nullptr*/, float volume /*= 1.0f*/)
{
	//path = std::string("sounds/gymwiththemonkey/") + path;
	while (path.substr(0, 1) == "/")
	{
		path = path.substr(1);
	}
	if (m_soundMap.find(path) != m_soundMap.end()) {
		// playing
		if (finishCallback)
			finishCallback();
		return;
	}
	auto audioId = AudioEngine::play2d(path, loop, volume);
	if (audioId == AudioEngine::INVALID_AUDIO_ID) {
		if (finishCallback)
			finishCallback();
		return;
	}
	auto result = m_soundMap.insert(std::pair<std::string, int>(path, audioId));
	if (!loop) {
		AudioEngine::setFinishCallback(audioId, [=](int, std::string path) {
			auto it = m_soundMap.find(path);
			if (it != m_soundMap.end())
				it = m_soundMap.erase(it);
			if (finishCallback)
				finishCallback();
		});
	}
}

void SoundManager::playChain(std::vector<std::string> paths, std::function<void()> finishCallback, float volume, int startIndex)
{
	if (paths.size() == 0 || paths.size() <= startIndex) {
		if (finishCallback)
			finishCallback();
		return;
	}

	playSound(paths[startIndex], false, [=]() {
		playChain(paths, finishCallback, volume, startIndex + 1);
	});
}

void SoundManager::stopSound(std::string path)
{
	auto it = m_soundMap.find(path);
	if (it == m_soundMap.end()) {
		// no sound
		return;
	}
	AudioEngine::stop(it->second);
	m_soundMap.erase(it);
}

void SoundManager::stopAllSound()
{
	auto it = m_soundMap.begin();
	while (it != m_soundMap.end())
	{
		AudioEngine::stop(it->second);
		it = m_soundMap.erase(it);
	}
}
