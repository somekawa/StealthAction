#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(cocos2d::Vector<cocos2d::Node*>& player, std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);
	~Imp();

	static Imp* CreateImp(cocos2d::Vector<cocos2d::Node*>& player, std::unordered_map<std::string,
		std::vector<std::vector<std::shared_ptr<ActionRect>>>>&collider);

	void Action(void)override;
	void update(float delta)override;

	//CREATE_FUNC(Imp);

private:
	void(Imp::* updater_)(void);

	void Walk(void);
	void Run(void);
	void Attack(void);
	void Death(void);

};