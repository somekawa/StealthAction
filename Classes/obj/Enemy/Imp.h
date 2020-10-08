#include "obj/Enemy/Enemy.h"

class Imp :public Enemy
{
public:
	Imp(std::vector<std::shared_ptr<Player>>& player);
	~Imp();

	static Imp* CreateImp(std::vector<std::shared_ptr<Player>>& player);

	void Action(void);

	//CREATE_FUNC(Imp);

private:
	void(Imp::* updater_)(void);

	void Walk(void);
	void Run(void);
	void Attack(void);
	void Death(void);

};