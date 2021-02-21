#pragma once
#include <cocos2d.h>
#include <memory>
#include <map>
#include <string>
#include <list>
#include <vector>

#define EffectMaxNum 50

#define lpEffectMng EffectManager::GetInstance()

struct FXStruct
{
	cocos2d::Sprite* sprite_;
	// �̪�Ă̓����蔻���`���߼޼�݂��ޯ�����ފi�[�p�ϐ�
	cocos2d::Rect rect_;
	// ���ݱ�è�ޏ�Ԃ�
	bool isActive_;
	// ٰ�ߍĐ����邩
	bool isLoop_;
	// ��Ұ��݂��I�����Ă��邩
	bool isAnimEnd_;
	// ��Ұ����ڰѶ��ĕϐ�
	float frameCnt_;
	// �̪�Ă��ړ������߰��
	cocos2d::Vec2 speed_;
	FXStruct() :sprite_(nullptr), isActive_(false), isLoop_(false), isAnimEnd_(false), frameCnt_(0.0f), speed_({ 0.0f,0.0f }) {};

	FXStruct(cocos2d::Sprite* sprite, bool active, bool loop,bool animEnd,float frame,cocos2d::Vec2 sp) :
		sprite_(sprite), isActive_(active), isLoop_(loop),isAnimEnd_(animEnd), frameCnt_(frame),speed_(sp) {};
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
	// �̪�ĉ摜�̐����Ǝ擾
	// �����I�ɂ́AEffectManager���̽��ײ��߰ق���쐬�����̪�ĉ摜�����o������
	// param@ effectName: �̪�Ė� frame: ���ڰѐ�
    //duration: 1ٰ�߂ɂ����鎞�� offset: �L��������sprite�ɂ�����̾�Ēl pos: �o���������߼޼��
	// flipFlag: ���E���]�����邩 loop: ٰ�ߍĐ������邩
	const FXStruct& PickUp(std::string effectName,
						   cocos2d::Vec2 offset, cocos2d::Vec2 pos, cocos2d::Vec2 speed,
						   int frame, float duration, bool flipFlag, bool loop, bool animReset);
	void Play(std::string effectName,cocos2d::Sprite* sprite, bool loop);

	// �X�V
	void Update(float delta);
	// �̪�Ă�active��Ԃ�false�ɂȂ�Ƌ��ɵ�޼ު���߰ق̒��̂��active��Ԃ�false�ɂ���
	void Reset(const FXStruct& fx);

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


	void Move(FXStruct fx);
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

