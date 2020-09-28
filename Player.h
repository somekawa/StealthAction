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
	bool keyActFlg_;					// idle画像にすべきか調べるフラグ

	PL_MOVE moveMode_;
	PL_DIR dir_;

	std::unique_ptr<Controller> controller_;	 // コントローラー
	std::map<InputID, std::function<void(std::map<InputID, TrgBool>&)>> keyAct_;	// キー別の動く処理
};

