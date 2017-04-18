*-- (c) Microsoft Corporation 1995

#INCLUDE "INCLUDE\TASTRADE.H"

*-- Instructions DECLARE DLL pour lire/�crire dans les fichiers priv�s INI
DECLARE INTEGER GetPrivateProfileString IN Win32API  AS GetPrivStr ;
  String cSection, String cKey, String cDefault, String @cBuffer, ;
  Integer nBufferSize, String cINIFile

DECLARE INTEGER WritePrivateProfileString IN Win32API AS WritePrivStr ;
  String cSection, String cKey, String cValue, String cINIFile

*-- Instructions DECLARE DLL pour lire/�crire dans le registre du syst�me
DECLARE Integer RegOpenKeyEx IN Win32API ;
  Integer nKey, String @cSubKey, Integer nReserved,;
  Integer nAccessMask, Integer @nResult

DECLARE Integer RegQueryValueEx IN Win32API ;
  Integer nKey, String cValueName, Integer nReserved,;
  Integer @nType, String @cBuffer, Integer @nBufferSize

DECLARE Integer RegCloseKey IN Win32API ;
  Integer nKey

*-- Instructions DECLARE DLL pour la fonction API GetProfileString dans Windows 3.1
DECLARE INTEGER GetProfileString IN Win32API AS GetProStr ;
  String cSection, String cKey, String cDefault, ;
  String @cBuffer, Integer nBufferSize

CLEAR

*-- Assure que le gestionnaire de projet est ferm�, sinon 
*-- des conflits peuvent appara�tre lorsqu'une touche rapide est tap�e (KEYBOARD)
DEACTIVATE WINDOW "Project Manager"

*-- Toutes les variables publiques sont supprim�es aussit�t
*-- que l'objet application est cr��e

IF SET('TALK') = 'ON'
  SET TALK OFF
  PUBLIC gcOldTalk
  gcOldTalk = 'ON'
ELSE
  PUBLIC gcOldTalk
  gcOldTalk = 'OFF'
ENDIF

PUBLIC gcOldDir, gcOldPath, gcOldClassLib, gcOldEscape, gTTrade
gcOldEscape   = SET('ESCAPE')
gcOldDir       = FULLPATH(CURDIR())
gcOldPath     = SET('PATH')
gcOldClassLib = SET('CLASSLIB')
gTTrade = .T.

*-- D�finit le chemin pour que l'objet Application puisse �tre instanci�
IF SetPath()
  PUBLIC oApp
  oApp = CREATEOBJECT("TasTrade")
  IF TYPE('oApp') = "O"
    *-- Enl�ve les variables publiques, puisque leurs valeurs
    *-- ont �t� r�cup�r�es par la classe Environment
    RELEASE gcOldDir, gcOldPath, gcOldClassLib, gcOldTalk, gcOldEscape
    oApp.Do()
  ENDIF
ENDIF

CLEAR DLLS
RELEASE ALL EXTENDED
CLEAR ALL

FUNCTION SetPath()
  LOCAL lcSys16, ;
        lcProgram

  lcSys16 = SYS(16)
  lcProgram = SUBSTR(lcSys16, AT(":", lcSys16) - 1)

  CD LEFT(lcProgram, RAT("\", lcProgram))
  *-- Si MAIN.PRG est ex�cut� directement, alors la d�tection
  *-- porteuse (Carrier Detect) se fait jusqu'au r�pertoire parent
  IF RIGHT(lcProgram, 3) = "FXP"
    CD ..
  ENDIF
  SET PATH TO PROGS, FORMS, LIBS, ;
        MENUS, DATA, OTHER, ;
        REPORTS, INCLUDE, HELP, ;
        BITMAPS
  SET CLASSLIB TO MAIN, TSGEN
ENDFUNC
