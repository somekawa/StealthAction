#pragma once
#include <cocos2d.h>
#include <memory>
#include <map>
#include <string>
#include <list>

#define EffectMaxNum 20

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
	void Update(void);
	// ｴﾌｪｸﾄを動かしたい場合はUpdate関数や、常に回る関数の中で書く
	// param@ sprite: ｴﾌｪｸﾄの画像ｽﾌﾟﾗｲﾄ speed: ｴﾌｪｸﾄの移動量
	void Move(cocos2d::Sprite* sprite, cocos2d::Vec2 speed);
	// ｴﾌｪｸﾄ画像の生成と取得
	// 内部的には、EffectManager内のｽﾌﾟﾗｲﾄﾌﾟｰﾙから作成したｴﾌｪｸﾄ画像を取り出す処理
	cocos2d::Sprite* createEffect(std::string effectName, int frame, float duration, cocos2d::Vec2 offset,cocos2d::Vec2 pos);
	// ｴﾌｪｸﾄの再生
	// この関数を各ｺﾝｽﾄﾗｸﾀで1回のみ呼び出せばよい
	void Play(cocos2d::Sprite* sprite,std::string effectName);
	// ｱﾆﾒｰｼｮﾝが終了したかのﾌﾗｸﾞ取得
	// 今のとこ使用していない
	const bool& GetAnimEnd(void)
	{
		return isAnimEnd_;
	}
	// ｵﾌﾞｼﾞｪｸﾄﾌﾟｰﾘﾝｸﾞするためのﾌﾟｰﾙ作成
	void CreatePools(cocos2d::Layer& layer);
	// 左右反転ﾌﾗｸﾞのｾｯﾄ
	void SetFlip(bool flg);
	// 左右反転でｵﾌｾｯﾄのｾｯﾄ
	cocos2d::Vec2 GetFlipOffset(std::string effectName);
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
	// ｽﾌﾟﾗｲﾄをためておくﾌﾟｰﾙ
	cocos2d::Vector<cocos2d::Sprite*>* spritePool_;
	// 上記のﾌﾟｰﾙの中で何番目のﾌﾟｰﾙを使用するかの番号
	int poolNo_;
	// ﾌﾟﾚｲﾔｰ、ｴﾈﾐｰの向きによって反転させるかしないかのﾌﾗｸﾞ
	bool flipFlag_;
};

