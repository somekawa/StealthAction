#pragma 
#include <memory>
#include <map>
#include <functional>
#include "input/Controller.h"
#include "input/KeyBoard.h"
#include "input/InputID.h"

enum class PL_MOVE {
	IDLE,
	DOWN,
	LEFT,
	RIGHT,
	MAX
};

enum class PL_DIR {
	LEFT,
	RIGHT,
	MAX
};

class Player
{
public:
	Player();
	~Player();
	void Draw(void);
	void Update(void);
private:
	bool Init(void);

	int playerIdleImg[4];
	int playerRunImg[10];
	int playerDown;
	int ImgMax_;
	int ImgNowCnt_;
	int animCnt_;
	bool keyActFlg_;					// idle�摜�ɂ��ׂ������ׂ�t���O

	PL_MOVE moveMode_;
	PL_DIR dir_;

	std::unique_ptr<Controller> controller_;	 // �R���g���[���[
	std::map<InputID, std::function<void(std::map<InputID, TrgBool>&)>> keyAct_;	// �L�[�ʂ̓�������
};

