#include "StdH.h"
#include "LuaFileLoader.h"
#include <Engine/Base/Stream.h>

bool CLoadLua::Lua_Load(const char* fileFath, lua_State* L)
{
	if (L == NULL)
		return false;

	// ���� �ε� & �ʱ�ȭ
	if (luaL_loadfile(L, fileFath) != 0 || lua_pcall(L, 0, 0, 0) != 0)
		return false;

	return true;
}

int CLoadLua::readDataToInteger( const char* fileFath, const char* fieldName )
{
	if (fieldName == NULL)
		return 0;

	// ��� ����
	lua_State* L = luaL_newstate();

	// ���� �ε�
	if(Lua_Load(fileFath, L) == false)
	{
		lua_close(L);
		return 0;
	}

	// ��� ���ÿ� ���� �̸��� ���� �ִ´�.
	lua_getglobal(L, fieldName);

	// ��� ������ �ε��� (plus, minus �ε����� �����ϸ� �ΰ��� ���ȴ�.)
	// top	   2	     -1
	// bottom  1     -2
	int nReturnVal = lua_tointeger(L, -1);
	// ��������� �ݾ��ش�.
	lua_close(L);

	return nReturnVal;
}

CTString CLoadLua::readDataToString( const char* fileFath, const char* fieldName )
{
	CTString strReturn = CTString("");

	if (fieldName == NULL)
		return strReturn;

	lua_State* L = luaL_newstate();

	if(Lua_Load(fileFath, L) == false)
	{
		lua_close(L);
		return strReturn;
	}

	lua_getglobal(L, fieldName);
	const char* strTemp = lua_tolstring(L, -1, NULL);
	strReturn = strTemp;
	lua_close(L);

	return strReturn;
}
