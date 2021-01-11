#pragma once
#include <cocos2d.h>
#include <memory>
#include <map>
#include <string>
#include <list>

#define lpEffectMng EffectManager::GetInstance()

// effectﾃﾞｰﾀ保存用ｸﾗｽ
// ﾃﾞｰﾀをaddしたりﾃﾞｰﾀを管理する
// 呼び出す時にnullchackをするようにする
class EffectManager
{
public:
	static EffectManager& GetInstance()
	{
		static EffectManager s_instance;
		return s_instance;
	}

	// 更新
	void Update(const cocos2d::Layer& layer);
	// ｴﾌｪｸﾄの追加
	// param@ effectName: ｴﾌｪｸﾄ名 
    //frame: ｱﾆﾒｰｼｮﾝﾌﾚｰﾑ数 
    //duration: 1ｺﾏに使用する時間 
	//offset: ｴﾌｪｸﾄ毎に設定するｵﾌｾｯﾄ値 
	//layer: ｴﾌｪｸﾄをぶら下げるﾚｲﾔｰ
	//isMove: ｴﾌｪｸﾄを動かすか
	void AddEffect(std::string effectName,int frame,float duration,cocos2d::Vec2 offset,cocos2d::Layer& layer,bool isMove = false);
	cocos2d::Sprite* AddEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset);
	void runaction(std::string effectName);
	// ｴﾌｪｸﾄの再生
	//void Play(const EffectType& eType, cocos2d::Vec2 pos);
	// ｴﾌｪｸﾄのﾛｰﾄﾞ
	//void Load(EffectType eType,int frame,float duration);
	// ｴﾌｪｸﾄｱﾆﾒｰｼｮﾝのｷｬｯｼｭの取得
	/*const cocos2d::Animation* GetAnimation(const EffectType& eType)
	{
		return effectAnimation_[eType];
	}*/
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ取得
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}

private:
	EffectManager() = default;
	~EffectManager();
	void operator=(EffectManager&) = delete;

	// ｱﾆﾒｰｼｮﾝのコマ数ｶｳﾝﾄ変数
	float animFrame_;
	// ｴﾌｪｸﾄに応じて発生させたいﾎﾟｼﾞｼｮﾝを変更させるためのoffset値
	std::map<std::string,cocos2d::Vec2> offset_;

	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ
	// 終了している:true
	// 終了していない:false
	bool isAnimEnd_;
	// 再生するｴﾌｪｸﾄのｱﾆﾒｰｼｮﾝﾃﾞｰﾀ
	cocos2d::Animate* animate_;
	// ｴﾌｪｸﾄのｽﾌﾟﾗｲﾄ
	std::map<std::string, cocos2d::Sprite> effectSprites_;
	// ｴﾌｪｸﾄ毎のｱﾆﾒｰｼｮﾝﾃﾞｰﾀ
	std::map<std::string, cocos2d::Animation*> effectAnimation_;
	// 再生可能なｴﾌｪｸﾄのﾘｽﾄ
	std::list<std::pair<cocos2d::Animation*,bool>> playList_;
	// ｴﾌｪｸﾄを再生するﾎﾟｼﾞｼｮﾝ
	cocos2d::Vec2 pos_;


};

