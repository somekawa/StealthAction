#pragma once
#include <cocos2d.h>
#include <memory>
#include <map>
#include <string>
#include <list>

#define lpEffectMng EffectManager::GetInstance()

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
	void Update(const cocos2d::Layer& layer);
	// �̪�Ă̒ǉ�
	// param@ effectName: �̪�Ė� 
    //frame: ��Ұ����ڰѐ� 
    //duration: 1�ςɎg�p���鎞�� 
	//offset: �̪�Ė��ɐݒ肷��̾�Ēl 
	//layer: �̪�Ă��Ԃ牺����ڲ԰
	//isMove: �̪�Ă𓮂�����
	void AddEffect(std::string effectName,int frame,float duration,cocos2d::Vec2 offset,cocos2d::Layer& layer,bool isMove = false);
	cocos2d::Sprite* AddEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset);
	void runaction(std::string effectName);
	// �̪�Ă̍Đ�
	//void Play(const EffectType& eType, cocos2d::Vec2 pos);
	// �̪�Ă�۰��
	//void Load(EffectType eType,int frame,float duration);
	// �̪�ı�Ұ��̷݂�����̎擾
	/*const cocos2d::Animation* GetAnimation(const EffectType& eType)
	{
		return effectAnimation_[eType];
	}*/
	// ��Ұ��݂��I�����������׸ގ擾
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}

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


};

