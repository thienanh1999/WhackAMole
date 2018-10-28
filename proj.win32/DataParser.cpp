#include "DataParser.h"
#include <cocos2d.h>
#include <fstream>

USING_NS_CC;
using namespace std;

void DataParser::init()
{
	this->mydata = "";

	ifstream fi;
	fi.open(this->jsonFileName.c_str(), ios::in);

	string line;
	string temp = "";
	while (getline(fi, line)) {
		temp += line;
	}

	fi.close();

	for (auto s : temp) {
		if (s != '\t') {
			this->mydata += s;
		}
	}
	this->doc.Parse<0>(this->getData());
}

const char* DataParser::getData() {
	return (this->mydata).c_str();
}

int DataParser::getNumQuest() {
	return this->doc[this->QuestionNumber].GetInt();
}

void DataParser::parseData(Question *ques, const int &level) {
	rapidjson::Value& currentlevel = this->doc[this->data][level];

	ques->numMole = (int)currentlevel[this->numMole].GetInt();
	ques->rightSound = currentlevel[this->rightSound].GetString();
	ques->sound = currentlevel[this->sound].GetString();
	ques->gamelevel = (int)currentlevel[this->gameLevel].GetInt();
}