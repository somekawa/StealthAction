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

 // �ꕔ�S���ꏊ
#include <vector>
#include <memory>
#include <array>
#include <list>
#include <unordered_map>
#include <utility>
#include <cocos2d.h>
#include "EnemyManager.h"
#include "anim/AnimMng.h"
#include "BaseScene.h"
#include "BehaviorBaseAI/BehaviorTree.h"

class ActionRect;
class EnemyManager;
using SharedRect = std::vector<std::shared_ptr<ActionRect>>;

 // ���s��
enum class zOrder {
    BG,
    CHAR_PL, 
	CHAR_ENEMY,
    FRONT,
    EFFECT,
    DEBUG,
    MAX
};

class GameMap;
class CameraManager;
class Player;
class PlayerShader;
class ResShadowShader;

class Game : public BaseScene
{
public:
    static cocos2d::Scene* createScene();
    ~Game();

    bool init()override;
    void update(float sp)override;
    
    CREATE_FUNC(Game);

	// �ڑ��v���C���[���Ƀv���C���[��ǉ�
	// param@ playerNum : �ڑ����Ă���v���C���[�̐�
	void AddPlayer(int playerNum);
private:
    // �V�F�[�_
    std::shared_ptr<PlayerShader> playerShader_;
    std::shared_ptr<ResShadowShader> resShadowShader_;
    // �J����
    std::shared_ptr<CameraManager> cameraManager_;
    // �}�b�v
    std::shared_ptr<GameMap> gameMap_;
	// ���C���[
	std::array<cocos2d::Layer*, static_cast<int>(zOrder::MAX)> layer_;

	std::array<cocos2d::Sprite*, static_cast<int>(ActorType::Max)> spriteList_;

	// �G�𐶐�������
	int generateEnemyNum_;
	// ���X�|�[������t���O
	bool respawnFlag_;

    BehaviorTree assassinBehavior_;
    BehaviorTree twistedCultistBehavior_;
    BehaviorTree cultistBehavior_;
    // �G�𓝊�����}�l�[�W���[
    std::unique_ptr<EnemyManager> enemyManager_;
	std::array<std::unordered_map<std::string, std::vector<SharedRect>>,
		static_cast<int>(ActorType::Max)> colliderBox_;

	// �J�ڒ����ǂ���
	bool isChanged_;			
	// �X�L��CT�\���̍X�V�֐�
	void SkillCntUpdate(std::string skillName,std::string buttonName, int skillNum);
	// �X�L��CT�̕\���p(first:old second:now)
	std::vector<std::pair<int, int>> skillTimeCnt_;			
};
#endif // __HELLOWORLD_SCENE_H__