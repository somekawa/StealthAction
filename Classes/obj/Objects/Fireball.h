#pragma once
#include "obj/Objects/Obj.h"
#include "Direction/Direction.h"

class Actor;

class Fireball :
    public Obj
{
public:
    Fireball(cocos2d::Vec2 pos,Direction direction,float angle,Actor& target);
    ~Fireball();

    void update(float delta);
    void Move(void);
    void AnimRegistrator(void);
    static Fireball* CreateFireball(cocos2d::Vec2 pos,Direction direction,float angle,Actor& target);

private:
    // 移動するｽﾋﾟｰﾄﾞ
    cocos2d::Vec2 speed_;
    // 左を向いていたらfalse
    // 右を向いていたらtrue
    bool dirFlag_;

};

