/***************************************************************************
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       SRLcom.cpp
 *                	SRL_ComWriteData	: Trace data out put
 *					SRL_ComWriteLog		: Dump data out put
 *  Content:    Lobby server & Lobby Client debug tools
 *
 ***************************************************************************/
//================================
// Include
//================================
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "LSPdebug.h"

/*=============================================================================
     Function�FDisplay the debug message
	 
     Input   �F	iLevel     �iTrace level : 0 - 3�j
				pszFormat  �iPointer to Message �j

     Output  �Fno

     Return  �FTrue 0
               False -1

==============================================================================*/
__declspec( dllexport )void __stdcall  LSP_Dbg_Printf( int iLevel, IN PTCHAR pszFormat, ... )
{
    TCHAR szDebugTmpStr[256];
    static TCHAR szDebugStr[256];
    SYSTEMTIME  lpst;
	
	memset(szDebugTmpStr, 0, sizeof(szDebugTmpStr));
	memset(szDebugStr, 0, sizeof(szDebugStr));

    va_list ap;
    va_start(ap, pszFormat);
    wvsprintf(szDebugTmpStr, pszFormat, ap);

	GetSystemTime(&lpst);
	wsprintf(szDebugStr,TEXT("%d-%d-%d %d:%d:%d-%dT(%ld):  %s"), lpst.wYear,
                                                                   lpst.wMonth,
                                                                   lpst.wDay,
                                                                   lpst.wHour,
                                                                   lpst.wMinute,
                                                                   lpst.wSecond,
                                                                   lpst.wMilliseconds,
                                                                   GetTickCount(),
                                                                   szDebugTmpStr);
    OutputDebugString(szDebugStr);
    va_end(ap);
	return;
}

/*=============================================================================
     Function�FGet registry information
	 
     Input   �FlpRegKey�iReg key)
		  	   lpReg   �iReg entry�j
			   lpBuff  �iPointer to Buf�j
			   dwSize  �isize�j
     Output  �FlpRegBuff
	 
     Return  �FTrue 0
               False -1

==============================================================================*/
int  LSP_Dbg_RegGetInfoBinary( LPCTSTR lpRegKey, LPCTSTR lpReg, LPCTSTR lpBuff, DWORD dwSize )
{
    HKEY    hKey;                 
	
    if( RegOpenKeyEx( HKEY_LOCAL_MACHINE, lpRegKey, 0, KEY_READ, &hKey ) != ERROR_SUCCESS )
		return( -1 );
	
    if( RegQueryValueEx( hKey, lpReg, NULL, NULL, (LPBYTE)lpBuff, &dwSize ) != ERROR_SUCCESS ){
        RegCloseKey( hKey );
		return( -1 );
    }
    RegCloseKey( hKey );
    return( 0 );
}
