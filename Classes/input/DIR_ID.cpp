#include "cocos2d.h"
#include "DIR_ID.h"

DIR begin(DIR)
{
	return DIR::NON;
}

DIR end(DIR)
{
	return DIR::MAX;		// endは実際の要素+1しないといけない
}

DIR operator*(DIR key)
{
	return key;
}

DIR operator++(DIR& key)
{
	return key = DIR(std::underlying_type< DIR >::type(key) + 1);
}

ACTION begin(ACTION)
{
	return ACTION::NON;
}

ACTION end(ACTION)
{
	return ACTION::MAX;		// endは実際の要素+1しないといけない
}

ACTION operator*(ACTION key)
{
	return key;
}

ACTION operator++(ACTION & key)
{
	return key = ACTION(std::underlying_type< ACTION >::type(key) + 1);
}