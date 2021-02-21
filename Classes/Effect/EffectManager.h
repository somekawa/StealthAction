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
	// ｴﾌｪｸﾄの当たり判定矩形のﾎﾟｼﾞｼｮﾝとﾎﾞｯｸｽｻｲｽﾞ格納用変数
	cocos2d::Rect rect_;
	// 現在ｱｸﾃｨﾌﾞ状態か
	bool isActive_;
	// ﾙｰﾌﾟ再生するか
	bool isLoop_;
	// ｱﾆﾒｰｼｮﾝが終了しているか
	bool isAnimEnd_;
	// ｱﾆﾒｰｼｮﾝﾌﾚｰﾑｶｳﾝﾄ変数
	float frameCnt_;
	// ｴﾌｪｸﾄが移動するｽﾋﾟｰﾄﾞ
	cocos2d::Vec2 speed_;
	FXStruct() :sprite_(nullptr), isActive_(false), isLoop_(false), isAnimEnd_(false), frameCnt_(0.0f), speed_({ 0.0f,0.0f }) {};

	FXStruct(cocos2d::Sprite* sprite, bool active, bool loop,bool animEnd,float frame,cocos2d::Vec2 sp) :
		sprite_(sprite), isActive_(active), isLoop_(loop),isAnimEnd_(animEnd), frameCnt_(frame),speed_(sp) {};
};

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
	// ｴﾌｪｸﾄ画像の生成と取得
	// 内部的には、EffectManager内のｽﾌﾟﾗｲﾄﾌﾟｰﾙから作成したｴﾌｪｸﾄ画像を取り出す処理
	// param@ effectName: ｴﾌｪｸﾄ名 frame: 総ﾌﾚｰﾑ数
    //duration: 1ﾙｰﾌﾟにかかる時間 offset: キャラ毎のspriteにかけるｵﾌｾｯﾄ値 pos: 出現させるﾎﾟｼﾞｼｮﾝ
	// flipFlag: 左右反転させるか loop: ﾙｰﾌﾟ再生させるか
	const FXStruct& PickUp(std::string effectName,
						   cocos2d::Vec2 offset, cocos2d::Vec2 pos, cocos2d::Vec2 speed,
						   int frame, float duration, bool flipFlag, bool loop, bool animReset);
	void Play(std::string effectName,cocos2d::Sprite* sprite, bool loop);

	// 更新
	void Update(float delta);
	// ｴﾌｪｸﾄのactive状態がfalseになると共にｵﾌﾞｼﾞｪｸﾄﾌﾟｰﾙの中のやつのactive状態もfalseにする
	void Reset(const FXStruct& fx);

	// ｴﾌｪｸﾄの再生(1回のみ)
	// この関数を各ｺﾝｽﾄﾗｸﾀで1回のみ呼び出せばよい
	void PlayWithOnce(FXStruct& fx,std::string effectName);
	// ｴﾌｪｸﾄ再生(ﾙｰﾌﾟ再生)
	void PlayWithLoop(FXStruct& fx, std::string effectName);
	// ｱﾆﾒｰｼｮﾝが終了したらtrueを返す関数
	// もしｱﾆﾒｰｼｮﾝが終了したら～をしたい、という時に使用してください。
	bool AnimEndChecker(FXStruct& fx,float delta);

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


	void Move(FXStruct fx);
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
	std::vector<FXStruct> spritePool_;
	// 上記のﾌﾟｰﾙの中で何番目のﾌﾟｰﾙを使用するかの番号
	int poolNo_;
	// ﾌﾟﾚｲﾔｰ、ｴﾈﾐｰの向きによって反転させるかしないかのﾌﾗｸﾞ
	bool flipFlag_;
};

