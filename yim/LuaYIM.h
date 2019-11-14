// CustomClass.h

#ifndef __CUSTOM__CLASS

#define __CUSTOM__CLASS

#include "cocos2d.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif
namespace cocos2d {
class CustomClass 
{
public:
    ~CustomClass();
	static CustomClass* getInstance();
private:
	CustomClass();
public:
	void Uninit();
	std::thread m_parseJsonThread;
	bool m_bUninit=false;
};
} //namespace cocos2d
TOLUA_API int register_youmeim_manual(lua_State* tolua_S);
#endif // __CUSTOM__CLASS