#pragma once
#include <array>
#include <map>

#include "Geometory.h"
#include "ActorType.h"
#include "Actor.h"
#include "RectSaver.h"
#include "Application.h"

constexpr int editScreenX_ = Screen_x / 2;
constexpr int editScreenY_ = Screen_y / 2;

class KeyInput;
class Mouse;

enum EditTurn
{
	Start,
	End,
	Max
};

class Scene
{
public:
	Scene();
	~Scene();

	void UpDate(const KeyInput& keyInput,const Mouse& mouse);

private:
	void Draw(void);
	void Init(void);

	// ｱﾆﾒｰｼｮﾝ名選択をさせるﾀｰﾝ
	void InputAnimation(void);

	// ﾌﾟﾚｲﾔｰか敵かを選ぶﾓｰﾄﾞ
	void SelectActor(const KeyInput& keyInput, const Mouse& mouse);
	// ｷｬﾗｸﾀｰのﾀｲﾌﾟの選択ﾓｰﾄﾞ
	void SelectActorType(const KeyInput& keyInput, const Mouse& mouse);
	// ｱﾆﾒｰｼｮﾝの選択ﾓｰﾄﾞ
	void SelectAnimation(const KeyInput& keyInput, const Mouse& mouse);
	// 矩形編集ﾓｰﾄﾞ
	void Edit(const KeyInput& keyInput, const Mouse& mouse);
	void SelectEditorLoad(const KeyInput& keyInput, const Mouse& mouse);
	// debugでsaveした矩形ﾃﾞｰﾀをﾛｰﾄﾞする
	void LoadRect(const KeyInput& keyInput, const Mouse& mouse);
	// 編集終了→SelectActorに戻る
	void End(const KeyInput& keyInput, const Mouse& mouse);
	// 内部のﾃﾞｰﾀをﾘｾｯﾄ
	void ResetData(void);

	bool rectLoadFlag_;

	void SelectActorDraw(void);
	void SelectActorTypeDraw(void);
	void SelectAnimationDraw(void);
	void EditDraw(void);
	void LoadRectDraw(void);
	void EndDraw(void);

	// ﾒﾝﾊﾞ関数ﾎﾟｲﾝﾀ
	void(Scene::*upDater_)(const KeyInput&, const Mouse&);
	void(Scene::* drawExecuter_)(void);

	Vector2I mousePos_;
	const Vector2I spritePos_;
	Vector2I begin_;
	Vector2I end_;

	// ﾌﾟﾚｲﾔｰor敵判別
	Actor actor_;
	// ﾌﾟﾚｲﾔｰかｴﾈﾐｰかの文字列
	// Maxだとｶﾞｲﾄﾞ表示
	std::array<std::string, static_cast<int>(Actor::Max)+1> actorName_;
	// 編集するｷｬﾗｸﾀｰﾀｲﾌﾟ
	ActorType actorType_;
	// ﾌﾟﾚｲﾔｰ、敵の各ﾀｲﾌﾟの名前
	std::array<std::string, static_cast<int>(ActorType::Max)> actorTypeName_;
	// 編集モードを切り替えたﾌﾗｸﾞ
	bool isModeChange_;
	// 画像ｱﾆﾒｰｼｮﾝを切り替えるｶｳﾝﾄ変数
	int animationCount_;
	// 1ｱﾆﾒｰｼｮﾝにある最大ﾌﾚｰﾑ数
	int maxFrame_;
	// 入力した文字列(ｱﾆﾒｰｼｮﾝ名)
	char inputAnimationName_[30];

	// actorの矩形情報を格納
	std::vector<SharedActionRect> actionRect_;
	// 1ﾌﾚｰﾑに存在する矩形の数
	std::vector<int> rectNum_;
	// 矩形ﾀｲﾌﾟ変数
	char rectType_;
	// 現在編集中のﾌﾚｰﾑ
	int currentFrame_;

	EditTurn turn_;
};

