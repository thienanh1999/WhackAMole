//
//  GAFWrapper.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/22/17.
//
//

#ifndef GAFWrapper_hpp
#define GAFWrapper_hpp

#include <stdio.h>
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"

class GAFWrapper : public cocos2d::ui::Layout {
public:
    static GAFWrapper * createGAF(const std::string &gaf_name);
    static GAFWrapper * createGAF(const std::string &gaf_name, cocos2d::Node * placeholder);
    static GAFWrapper * createGAF(gaf::GAFAsset *asset);

    void play(bool looped, std::function<void(gaf::GAFObject* obj)> callback = nullptr);
    void playSequence(std::string sequence, bool looped = false, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback = nullptr);
    void playSequenceWithPause(std::string sequence, bool looped = false, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback = nullptr);
    void setContentSize(const cocos2d::Size & size) override;
    void reset();
    void forceLoop(bool is_loop, std::function<void(gaf::GAFObject * obj)> callback = nullptr);
    void pauseAnimation();
    void resumeAnimation();
    void goToEndFrame();
    void setFps(int fps);
	virtual bool init(const std::string &gaf_name);
	virtual void onExit() override;
private:
    bool init(const std::string & gaf_name, cocos2d::Node * placeholder);
    bool init(gaf::GAFAsset *asset);
    gaf::GAFObject * _object = nullptr;
    std::string schedule_name;
    
};

#endif /* GAFWrapper_hpp */
