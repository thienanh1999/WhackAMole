#include "Mole.h"
#include "Constant.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

Mole* Mole::Init()
{
	//auto mole = GAFWrapper::createGAF("D:/study/Cocos/WhackAMole/Resources/mole/mole.gaf");
	auto mole = new Mole();
	mole->init("D:/study/Cocos/WhackAMole/Resources/mole/mole.gaf");
	mole->Ground();
	mole->setScale(0.75f);
	mole->layout = Layout::create();
	mole->layout->setSize(Size(200.0f,200.0f));
	//mole->layout->setBackGroundColor(Color3B::GREEN);
	//mole->layout->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	//mole->layout->setVisible(true);
	mole->layout->setOpacity(100);
	mole->inQueue = false;
	return mole;
}

void Mole::Up()
{
	//play sound
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("sound/up.mp3", false);
	
	//up
	this->playSequence("up", false, [=](gaf::GAFObject * obj, const std::string& sqN) {
		this->Idle();
	});
	this->isUp = true;
	this->textField->setVisible(true);
	auto fadeIn = FadeIn::create(1.0f);
	this->textField->runAction(fadeIn);
}

void Mole::Down()
{
	this->isUp = false;
	auto fadeOut = FadeOut::create(0.0f);
	this->textField->setVisible(false);
	this->textField->runAction(fadeOut);
	this->playSequence("down", false, [=](gaf::GAFObject * obj, const std::string& abc) {
		this->Ground();
	});
}

void Mole::Hit()
{
	//play sound
	auto audio = SimpleAudioEngine::getInstance();
	audio->playEffect("sound/hit.mp3", false);
	auto sound1 = SimpleAudioEngine::getInstance();
	sound1->playEffect(this->sound.c_str(), false);

	//hit
	this->isUp = false;
	this->Correct();
	this->playSequence("hit", false, [=](gaf::GAFObject* obj, const std::string& abc) {
		this->Ground();
	});
}

void Mole::Idle()
{
	this->playSequence("idle", true);
}

void Mole::Ground()
{
	this->playSequence("ground", true);
}

void Mole::place(float x, float y)
{
	//set position
	this->setPosition(Vec2(x,y));

	//add a layout
	this->layout->setPosition(Vec2(x-130,y-150));

	//add text
	this->textField->setPosition(Vec2(x - 20, y));
	this->posx = x - 20;
	this->posy = y;
}

void Mole::setText(std::string text)
{
	this->textField = TextFieldTTF::create(text, "Arial", textSize);
	this->textField->setColor(Color3B::BLACK);
	this->textField->setVisible(false);
	auto fadeOut = FadeOut::create(0.0f);
	this->textField->runAction(fadeOut);
	this->textField->setLocalZOrder(100);
}

void Mole::Correct()
{
	auto moveBy = MoveBy::create(1, Vec2(0, 50));
	this->textField->runAction(Sequence::create(moveBy, CallFunc::create([=] {
		this->textField->setVisible(false);
		this->textField->setPosition(Vec2(this->posx, this->posy));
	}), NULL));
	auto fadeOut = FadeOut::create(1.0f);
	this->textField->runAction(fadeOut);
}
