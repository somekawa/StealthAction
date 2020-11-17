#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(cocos2d::Vector<cocos2d::Node*>& player);
	~Imp();

	static Imp* CreateImp(cocos2d::Vector<cocos2d::Node*>& player);

	void Action(void)override;
	void update(float delta)override;
	void AnimRegistrator(void);
	//CREATE_FUNC(Imp);

private:
	void(Imp::* updater_)(void);

	void Walk(void);
	void Run(void);
	void Attack(void);
	void Death(void);

};