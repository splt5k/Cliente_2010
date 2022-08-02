#include "stdh.h"
#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/Base/Registry.h>

static void ParseKeyName(const CTString &strKey, HKEY &hKeyRoot, CTString &strKeyPath, CTString &strKeyName)
{
	CTString strRemain = strKey;
	// separate key into the value part and path
	strKeyName	= CTFileName(strRemain).FileName();
	strRemain	= CTFileName(strRemain).FileDir();
	strRemain.TrimRight(strlen(strRemain)-1); // removes trailing backslash
	// try to find root key value
	
	if (strRemain.RemovePrefix("HKEY_CLASSES_ROOT\\")) 
	{
		hKeyRoot = HKEY_CLASSES_ROOT;
	} 
	else if (strRemain.RemovePrefix("HKEY_CURRENT_CONFIG\\"))
	{
		hKeyRoot = HKEY_CURRENT_CONFIG;
	} 
	else if (strRemain.RemovePrefix("HKEY_CURRENT_USER\\")) 
	{
		hKeyRoot = HKEY_CURRENT_USER;
	} 
	else if (strRemain.RemovePrefix("HKEY_LOCAL_MACHINE\\")) 
	{
		hKeyRoot = HKEY_LOCAL_MACHINE;
	} 
	else if (strRemain.RemovePrefix("HKEY_USERS\\")) 
	{
		hKeyRoot = HKEY_USERS;
	} 
	else 
	{
		ASSERT(FALSE);
		hKeyRoot = HKEY_CURRENT_USER;
	}
	
	strKeyPath = strRemain;
}

ENGINE_API BOOL REG_GetString(const CTString &strKey, CTString &strString)
{
	// parse the key name
	HKEY hKeyRoot; CTString strKeyPath; CTString strKeyName;
	ParseKeyName(strKey, hKeyRoot, strKeyPath, strKeyName);
	
	// open the key
	HKEY hkey;
	LONG lRes = RegOpenKeyEx(hKeyRoot, strKeyPath, 0, KEY_READ, &hkey);
	if (lRes!=ERROR_SUCCESS) {
		return FALSE;
	}
	
	// get the value
	char achrKeyValue[1024];
	ULONG ctType;
	ULONG ctLength = sizeof(achrKeyValue);
	LONG lResult = RegQueryValueEx(hkey, strKeyName, 0, &ctType, (unsigned char *)achrKeyValue, &ctLength);
	if(lResult != ERROR_SUCCESS) {
		return FALSE;
	}
	
	// close the key
	RegCloseKey(hkey);
	
	strString = CTString( achrKeyValue);
	return TRUE;
}

ENGINE_API BOOL REG_GetLong(const CTString &strKey, ULONG &ulLong)
{
	// parse the key name
	HKEY hKeyRoot; CTString strKeyPath; CTString strKeyName;
	ParseKeyName(strKey, hKeyRoot, strKeyPath, strKeyName);
	
	// open the key
	HKEY hkey;
	LONG lRes = RegOpenKeyEx(hKeyRoot, strKeyPath, 0, KEY_READ, &hkey);
	if (lRes!=ERROR_SUCCESS) {
		return FALSE;
	}
	
	// get the value
	ULONG ulKeyValue;
	ULONG ctType;
	ULONG ctLength = sizeof(ulKeyValue);
	LONG lResult = RegQueryValueEx(hkey, strKeyName, 0, &ctType, (unsigned char *)&ulKeyValue, &ctLength);
	if(lResult != ERROR_SUCCESS) {
		return FALSE;
	}
	
	// close the key
	RegCloseKey(hkey);
	
	ulLong = ulKeyValue;
	return TRUE;
}

ENGINE_API BOOL REG_SetString(const CTString &strKey, const CTString &strString)
{
	// parse the key name
	HKEY hKeyRoot; CTString strKeyPath; CTString strKeyName;
	ParseKeyName(strKey, hKeyRoot, strKeyPath, strKeyName);
	
	// create the registry key
	HKEY hkey;
	DWORD dwDisposition;
	LONG lRes = RegCreateKeyEx(hKeyRoot, (const char*)strKeyPath, 0,
		"", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dwDisposition);
	if (lRes!=ERROR_SUCCESS) 
	{
		return FALSE;
	}
	
	// set the value
	lRes = RegSetValueEx(hkey, strKeyName, 0, REG_SZ, (const UBYTE*)(const char*)strString, strlen(strString));
	
	// close the key
	RegCloseKey(hkey);
	
	return lRes==ERROR_SUCCESS;
}

