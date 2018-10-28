#pragma once

#include "cocos2d.h"

class SoundManager
{
public:
	static SoundManager* getInstance();
	void playSound(std::string path, bool loop = false, std::function<void()> finishCallback = nullptr, float volume = 1.0f);
	void playChain(std::vector<std::string> paths, std::function<void()> finishCallback = nullptr, float volume = 1.0f, int startIndex = 0);
	void stopSound(std::string path);
	void stopAllSound();
protected:
	std::map<std::string, int> m_soundMap;
private:
	SoundManager();
	static SoundManager* m_instance;
};