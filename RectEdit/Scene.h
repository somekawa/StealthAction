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

	// ��Ұ��ݖ��I�������������
	void InputAnimation(void);

	// ��ڲ԰���G����I��Ӱ��
	void SelectActor(const KeyInput& keyInput, const Mouse& mouse);
	// ��׸�������߂̑I��Ӱ��
	void SelectActorType(const KeyInput& keyInput, const Mouse& mouse);
	// ��Ұ��݂̑I��Ӱ��
	void SelectAnimation(const KeyInput& keyInput, const Mouse& mouse);
	// ��`�ҏWӰ��
	void Edit(const KeyInput& keyInput, const Mouse& mouse);
	void SelectEditorLoad(const KeyInput& keyInput, const Mouse& mouse);
	// debug��save������`�ް���۰�ނ���
	void LoadRect(const KeyInput& keyInput, const Mouse& mouse);
	// �ҏW�I����SelectActor�ɖ߂�
	void End(const KeyInput& keyInput, const Mouse& mouse);
	// �������ް���ؾ��
	void ResetData(void);

	bool rectLoadFlag_;

	void SelectActorDraw(void);
	void SelectActorTypeDraw(void);
	void SelectAnimationDraw(void);
	void EditDraw(void);
	void LoadRectDraw(void);
	void EndDraw(void);

	// ���ފ֐��߲��
	void(Scene::*upDater_)(const KeyInput&, const Mouse&);
	void(Scene::* drawExecuter_)(void);

	Vector2I mousePos_;
	const Vector2I spritePos_;
	Vector2I begin_;
	Vector2I end_;

	// ��ڲ԰or�G����
	Actor actor_;
	// ��ڲ԰����а���̕�����
	// Max���ƶ޲�ޕ\��
	std::array<std::string, static_cast<int>(Actor::Max)+1> actorName_;
	// �ҏW���鷬׸������
	ActorType actorType_;
	// ��ڲ԰�A�G�̊e���߂̖��O
	std::array<std::string, static_cast<int>(ActorType::Max)> actorTypeName_;
	// �ҏW���[�h��؂�ւ����׸�
	bool isModeChange_;
	// �摜��Ұ��݂�؂�ւ��鶳�ĕϐ�
	int animationCount_;
	// 1��Ұ��݂ɂ���ő��ڰѐ�
	int maxFrame_;
	// ���͂���������(��Ұ��ݖ�)
	char inputAnimationName_[30];

	// actor�̋�`�����i�[
	std::vector<SharedActionRect> actionRect_;
	// 1�ڰтɑ��݂����`�̐�
	std::vector<int> rectNum_;
	// ��`���ߕϐ�
	char rectType_;
	// ���ݕҏW�����ڰ�
	int currentFrame_;

	EditTurn turn_;
};

