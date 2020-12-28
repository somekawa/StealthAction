/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <vector>
#include <memory>
#include <array>
#include <list>
#include <unordered_map>
#include "cocos2d.h"
#include "anim/AnimMng.h"
#include "BaseScene.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class ActionRect;
using SharedRect = std::vector<std::shared_ptr<ActionRect>>;

// 単語の綴りが違う
// 書くなら "zOrder"　ここ要変更！
 // zOlder(奥行き)
enum class zOlder {
    BG,
    CHAR_PL, 
	CHAR_ENEMY,
    FRONT,
    DEBUG,
    MAX
};

class GameMap;
class CameraManager;
class Player;
class ProgramState;
class Game : public BaseScene
{
public:
    static cocos2d::Scene* createScene();
    ~Game();
    bool init()override;
    void update(float sp)override;

    // Player* plSprite;
    cocos2d::Sprite* enemySprite;

    cocos2d::TMXLayer* colLayerRemain;
    std::shared_ptr<CameraManager> cameraManager_;
    std::shared_ptr<GameMap> gameMap_;
    int frame = 0;
    float zoom = 1;

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    // 接続プレイヤー毎にプレイヤーを追加
    // param@ playerNum: 接続しているプレイヤーの数
    void AddPlayer(int playerNum);
    // 敵の生成
    void AddEnemy(const ActorType& type);

    // implement the "static create()" method manually
    CREATE_FUNC(Game);
private:
    // レイヤー
    std::array<cocos2d::Layer*, static_cast<int>(zOlder::MAX)> layer_;

    std::array<cocos2d::Sprite*, static_cast<int>(ActorType::Max)> spriteList_;

    //std::list<std::shared_ptr<UIBase>> gameUIs_;
    // Titleに戻るための処理
    //void ChangeTitleScene(void);
    // 敵を生成した回数
    int generateEnemyNum_;
    // リスポーンするフラグ
    bool respawnFlag_;

    BehaviorTree assassinBehavior_;
    BehaviorTree twistedCultistBehavior_;
    BehaviorTree cultistBehavior_;

    std::array<std::unordered_map<std::string, std::vector<SharedRect>>,
        static_cast<int>(ActorType::Max)> colliderBox_;
};
#endif // __HELLOWORLD_SCENE_H__