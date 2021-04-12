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
    // ˆÚ“®‚·‚é½Ëß°ÄŞ
    cocos2d::Vec2 speed_;
    bool dirFlag_;			// true:‰EŒü‚« false:¶Œü‚«
};