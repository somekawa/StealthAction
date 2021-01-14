#pragma once
#include <cocos2d.h>
#include <memory>
#include <map>
#include <string>
#include <list>
#include <vector>

#define EffectMaxNum 20

#define lpEffectMng EffectManager::GetInstance()

struct FXStruct
{
	cocos2d::Sprite* sprite_;
	bool isActive_;
	bool isLoop_;
	bool isAnimEnd_;
	float frameCnt_;
	FXStruct() :sprite_(nullptr), isActive_(false), isLoop_(false), isAnimEnd_(false), frameCnt_(0.0f) {};

	FXStruct(cocos2d::Sprite* sprite, bool active, bool loop,bool animEnd, float frame) :
		sprite_(sprite), isActive_(active), isLoop_(loop),isAnimEnd_(animEnd), frameCnt_(frame) {};
};

// effect�ް��ۑ��p�׽
// �ް���add�������ް����Ǘ�����
// �Ăяo������nullchack������悤�ɂ���
class EffectManager
{
public:
	static EffectManager& GetInstance()
	{
		static EffectManager s_instance;
		return s_instance;
	}

	// �X�V
	void Update(float delta);
	// �̪�Ă𓮂��������ꍇ��Update�֐���A��ɉ��֐��̒��ŏ���
	// param@ sprite: �̪�Ẳ摜���ײ� speed: �̪�Ă̈ړ���
	// speed�͂��̊֐��̒��Ō������Ƃ� - or + �����肵�Ă���̂ŁA�P�Ɉړ��ʂ������Ă����Α��v
	void Move(cocos2d::Sprite* sprite, cocos2d::Vec2 speed);
	// �̪�ĉ摜�̐����Ǝ擾
	// �����I�ɂ́AEffectManager���̽��ײ��߰ق���쐬�����̪�ĉ摜�����o������
	const FXStruct& createEffect(std::string effectName,
								 int frame, float duration,
								 cocos2d::Vec2 offset,cocos2d::Vec2 pos,
							     bool flipFlag,bool loop = false);
	// �̪�Ă̍Đ�(1��̂�)
	// ���̊֐����e�ݽ�׸���1��̂݌Ăяo���΂悢
	void PlayWithOnce(FXStruct& fx,std::string effectName);
	// �̪�čĐ�(ٰ�ߍĐ�)
	void PlayWithLoop(FXStruct& fx, std::string effectName);
	// ��Ұ��݂��I��������true��Ԃ��֐�
	// ������Ұ��݂��I��������`���������A�Ƃ������Ɏg�p���Ă��������B
	bool AnimEndChecker(FXStruct& fx,float delta);

	// ��Ұ��݂��I�����������׸ގ擾
	// ���̂Ƃ��g�p���Ă��Ȃ�
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}
	// ��޼ު���߰�ݸނ��邽�߂��ٍ߰쐬
	void CreatePools(cocos2d::Layer& layer);
	// ���E���]�׸ނ̾��
	void SetFlip(bool flg);
	// ���E���]�ŵ̾�Ă̾��
	cocos2d::Vec2 GetFlipOffset(std::string effectName);
private:
	EffectManager() = default;
	~EffectManager();
	void operator=(EffectManager&) = delete;

	// ��Ұ��݂̃R�}�����ĕϐ�
	float animFrame_;
	// �̪�Ăɉ����Ĕ������������߼޼�݂�ύX�����邽�߂�offset�l
	std::map<std::string,cocos2d::Vec2> offset_;

	// ��Ұ��݂��I�����������׸�
	// �I�����Ă���:true
	// �I�����Ă��Ȃ�:false
	bool isAnimEnd_;
	// �Đ�����̪�Ă̱�Ұ����ް�
	cocos2d::Animate* animate_;
	// �̪�Ă̽��ײ�
	std::map<std::string, cocos2d::Sprite> effectSprites_;
	// �̪�Ė��̱�Ұ����ް�
	std::map<std::string, cocos2d::Animation*> effectAnimation_;
	// �Đ��\�ȴ̪�Ă�ؽ�
	std::list<std::pair<cocos2d::Animation*,bool>> playList_;
	// �̪�Ă��Đ������߼޼��
	cocos2d::Vec2 pos_;
	// ���ײĂ����߂Ă����߰�
	std::vector<FXStruct> spritePool_;
	// ��L���߰ق̒��ŉ��Ԗڂ��߰ق��g�p���邩�̔ԍ�
	int poolNo_;
	// ��ڲ԰�A��а�̌����ɂ���Ĕ��]�����邩���Ȃ������׸�
	bool flipFlag_;
};

