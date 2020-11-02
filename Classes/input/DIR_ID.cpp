#include "cocos2d.h"
#include "DIR_ID.h"

DIR begin(DIR)
{
	return DIR::NON;
}

DIR end(DIR)
{
	return DIR::MAX;		// endÇÕé¿ç€ÇÃóvëf+1ÇµÇ»Ç¢Ç∆Ç¢ÇØÇ»Ç¢
}

DIR operator*(DIR key)
{
	return key;
}

DIR operator++(DIR& key)
{
	return key = DIR(std::underlying_type< DIR >::type(key) + 1);
}