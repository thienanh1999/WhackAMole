#pragma once
#include "json/document.h"
#include "json/rapidjson.h"
#include <string>
#include <iostream>
#include "Question.h"

class DataParser
{
public:
	void init();
	int getNumQuest();
	const char* getData();
	void parseData(Question *ques, const int &level);

protected:
	std::string mydata;
	rapidjson::Document doc;

private:
	const std::string jsonFileName = "data.json";
	const char* data = "data";
	const char* QuestionNumber = "QuestionNumber";
	const char* numMole = "numMole";
	const char* rightSound = "rightSound";
	const char* sound = "sound";
	const char* gameLevel = "gameLevel";
};