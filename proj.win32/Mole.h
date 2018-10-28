#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GAFWrapper.h"
#include "SimpleAudioEngine.h"
#include <string>

class Mole : public GAFWrapper
{
public:
	static Mole* Init();
	void Up();
	void Down();
	void Hit();
	void Idle();
	void Ground();
	bool isUp;
	bool inQueue;
	void place(float x, float y);
	cocos2d::ui::Layout* layout;
	cocos2d::Label* textField;
	void setText(std::string text);
	std::string sound;

protected:
	void Correct();
	float posx;
	float posy;
};
