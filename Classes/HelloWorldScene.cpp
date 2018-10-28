#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Constant.h"
#include "SoundManager.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;
using namespace std;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layout::init() )
    {
        return false;
    }
    
    this->rootNode = CSLoader::createNode("MainScene.csb");

	Size size = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(size);
	ui::Helper::doLayout(rootNode);

	this->question = new Question;
	this->currentLevel = 0;
	this->data = new DataParser;
	data->init();
	this->score = 0;

	addChild(rootNode);

    return true;
}

void HelloWorld::onEnter()
{
	Layout::onEnter();
	this->gameState = GameState::Ready;
	this->triggerReady();
}

void HelloWorld::resetGame() 
{
	//reset feild
	for (int i = 0; i < this->moles.size(); i++)
	{
		this->moles[i]->textField->removeFromParent();
		this->moles[i]->removeFromParent();
	}
	this->moles.clear();
	while (this->upMoles.size() != 0)
		this->upMoles.pop();
	auto bg = this->rootNode->getChildByName("bg");
	bg->setVisible(false);
}

void HelloWorld::triggerReady()
{
	this->resetGame();

	//get data
	this->getData();

	//play music
	auto audio = SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("sound/music.mp3",true);
	//audio->playEffect("dataSound/tap.mp3", false);
	
	auto sound = SoundManager::getInstance();
	vector<string> paths;
	paths.clear();
	paths.push_back("dataSound/tap.mp3");
	paths.push_back(this->question->sound);
	sound->playChain(paths, nullptr, 1.0f, 0);

	this->text = TextFieldTTF::create(this->question->rightSound, "Arial", 100);
	this->text->setColor(Color3B::WHITE);
	this->text->setPosition(400, 400);
	this->rootNode->addChild(this->text);

	this->runAction(Sequence::create
	(DelayTime::create(2.5), CallFunc::create([&]() { 
		auto bg = this->rootNode->getChildByName("bg");
		bg->setVisible(true);
		this->text->removeFromParent();

		// setup moles
		int level = 0;
		for (int i = 0; i < this->question->numMole; i++)
		{
			//set position
			auto mole = Mole::Init();
			mole->setText(this->question->rightSound);
			mole->sound = this->question->sound;
			level = i / 3;
			if (level == (this->question->numMole - 1) / 3)
			{
				if (this->question->numMole % 3 == 2)
					mole->place(200.0f + Xspace / 2 + (i % 3) * Xspace, 200.0f + level * Yspace);
				if (this->question->numMole % 3 == 1)
					mole->place(200.0f + Xspace, 200.0f + level * Yspace);
				if (this->question->numMole % 3 == 0)
					mole->place(200.0f + (i % 3) * Xspace, 200.0f + level * Yspace);
			}
			else
				mole->place(200.0f + (i % 3) * Xspace, 200.0f + level * Yspace);

			mole->setLocalZOrder(10 * (4 - level));

			this->moles.push_back(mole);
			this->rootNode->addChild(this->moles[i]);
			this->rootNode->addChild(this->moles[i]->layout);
			this->rootNode->addChild(this->moles[i]->textField);

			//add touch listener
			mole->layout->setTouchEnabled(true);
			mole->layout->addClickEventListener([=](Ref* ref)
			{
				if (mole->isUp)
				{
					mole->Hit();
					this->score++;
					if (this->score % 10 == 0)
						nextRound();
				}
			});
		}

		//switch gameState
		this->gameState = GameState::Playing;
		this->triggerPlaying();
		}),nullptr));
}

void HelloWorld::triggerPlaying()
{
	this->schedule(schedule_selector(HelloWorld::Update), gameSpeed);
}

void HelloWorld::Update(float dt)
{
	if (this->gameState == GameState::Playing)
	{
		this->switchMole();
	}
}

void HelloWorld::getData()
{
	//fuck logic
	this->data->parseData(this->question, this->currentLevel);
	this->currentLevel++;
	if (this->currentLevel == this->data->getNumQuest())
		this->currentLevel = 0;
	this->gameLevel = this->question->gamelevel;
}

void HelloWorld::switchMole()
{
	if (this->upMoles.size() != this->gameLevel)
	{
		for (int j = this->upMoles.size(); j < this->gameLevel; j++)
		{
			bool found = false;
			while (!found)
			{
				int i = cocos2d::random(0, this->question->numMole - 1);
				if (!this->moles[i]->isUp)
				{
					this->moles[i]->Up();
					this->upMoles.push(i);
					this->moles[i]->inQueue = true;
					found = true;
				}
			}
		}
		return;
	}
	
	bool found = false;
	while (!found)
	{
		int i = cocos2d::random(0, this->question->numMole - 1);
		if (!this->moles[i]->inQueue)
		{
			this->moles[i]->Up();
			this->upMoles.push(i);
			this->moles[i]->inQueue = true;
			found = true;
		}
	}
	if (this->moles[this->upMoles.front()]->isUp == true)
	{
		this->moles[this->upMoles.front()]->Down();
	}
	this->moles[this->upMoles.front()]->inQueue = false;
	this->upMoles.pop();
}

void HelloWorld::nextRound()
{
	this->triggerReady();
	this->unschedule(schedule_selector(HelloWorld::Update));
}
