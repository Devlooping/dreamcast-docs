# Microsoft Developer Studio Generated NMAKE File, Based on LSPcallback.dsp
!IF "$(CFG)" == ""
CFG=LSPcallback - Win32 (WCE SH4) Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� LSPcallback - Win32 (WCE SH4) Debug ��ݒ肵�܂��B
!ENDIF 

!IF "$(CESubsystem)" == ""
CESubsystem=windowsce,2.0
!MESSAGE �ϐ� CESubsystem ���w�肳��Ă��܂���B ����� windowsce,2.0 ���g�p���܂��B
!ENDIF 

!IF "$(CEVersion)" == ""
CEVersion=200
!MESSAGE �ϐ� CEVersion ���w�肳��Ă��܂���B ����� 2.00 ���g�p���܂��B
!ENDIF 

!IF "$(CEConfigName)" == ""
CEConfigName=H/PC Ver. 2.00
!MESSAGE �ϐ� CEConfigName ���w�肳��Ă��܂���B����� H/PC Ver. 2.00 ���g�p���܂��B
!ENDIF 

!IF $(CEVersion) < 201
CECrt=L
CECrtDebug=Ld
CECrtMT=T
CECrtMTDebug=Td
CENoDefaultLib=corelibc.lib
CEx86Corelibc= 
!ELSE 
CECrt=C
CECrtDebug=C
CECrtMT=C
CECrtMTDebug=C
CENoDefaultLib=libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib
CEx86Corelibc=corelibc.lib
!ENDIF 

!IF "$(CFG)" != "LSPcallback - Win32 (WCE SH4) Release" && "$(CFG)" != "LSPcallback - Win32 (WCE SH4) Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "LSPcallback.mak" CFG="LSPcallback - Win32 (WCE SH4) Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "LSPcallback - Win32 (WCE SH4) Release" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
!MESSAGE "LSPcallback - Win32 (WCE SH4) Debug" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
!MESSAGE 
!ERROR �����ȍ\�����w�肳��Ă��܂��B
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=shcl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LSPcallback - Win32 (WCE SH4) Release"

OUTDIR=.\WCESH4Rel
INTDIR=.\WCESH4Rel
# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

ALL : "$(OUTDIR)\LobbySP.dll"


CLEAN :
	-@erase "$(INTDIR)\LSPdplay.obj"
	-@erase "$(INTDIR)\LSPgroup.obj"
	-@erase "$(INTDIR)\LSPmakelist.obj"
	-@erase "$(INTDIR)\LSPplayer.obj"
	-@erase "$(INTDIR)\LSPsession.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LobbySP.dll"
	-@erase "$(OUTDIR)\LobbySP.exp"
	-@erase "$(OUTDIR)\LobbySP.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Ox /I "..\include" /I "c:\WCEDremcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPcallback.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /Qgvp /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPcallback.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib ..\lib\LSPdebug.lib ..\lib\LSPregistry.lib ..\lib\LSPsend.lib ..\lib\LSPmanage.lib dplayx.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:no /pdb:"$(OUTDIR)\LobbySP.pdb" /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\LSPcallback.def" /out:"$(OUTDIR)\LobbySP.dll" /implib:"$(OUTDIR)\LobbySP.lib" /libpath:"..\lib" /libpath:"c:\WCEDreamcast\lib\retail" /libpath:"..\..\..\DrgaonARK\lib\SH4\wince\retail" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\LSPcallback.def"
LINK32_OBJS= \
	"$(INTDIR)\LSPdplay.obj" \
	"$(INTDIR)\LSPgroup.obj" \
	"$(INTDIR)\LSPmakelist.obj" \
	"$(INTDIR)\LSPplayer.obj" \
	"$(INTDIR)\LSPsession.obj"

"$(OUTDIR)\LobbySP.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\LobbySP.dll"
   copy WCESH4Rel\LobbySP.dll d:\DragonARK\release\katana\SH4\wince\retail
	copy WCESH4Rel\LobbySP.dll ..\dll
	copy WCESH4Rel\LobbySP.lib ..\lib
	copy WCESH4Rel\LobbySP.dll c:\WCEDreamcast\release\retail
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "LSPcallback - Win32 (WCE SH4) Debug"

OUTDIR=.\WCESH4Dbg
INTDIR=.\WCESH4Dbg
# Begin Custom Macros
OutDir=.\WCESH4Dbg
# End Custom Macros

ALL : "$(OUTDIR)\LobbySP.dll"


CLEAN :
	-@erase "$(INTDIR)\LSPdplay.obj"
	-@erase "$(INTDIR)\LSPgroup.obj"
	-@erase "$(INTDIR)\LSPmakelist.obj"
	-@erase "$(INTDIR)\LSPplayer.obj"
	-@erase "$(INTDIR)\LSPsession.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LobbySP.dll"
	-@erase "$(OUTDIR)\LobbySP.exp"
	-@erase "$(OUTDIR)\LobbySP.ilk"
	-@erase "$(OUTDIR)\LobbySP.lib"
	-@erase "$(OUTDIR)\LobbySP.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Od /I "..\include" /I "c:\WCEDremcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPcallback.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPcallback.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib ..\lib\LSPdebug.lib ..\lib\LSPregistry.lib ..\lib\LSPsend.lib ..\lib\LSPmanage.lib dplayx.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:yes /pdb:"$(OUTDIR)\LobbySP.pdb" /debug /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\LSPcallback.def" /out:"$(OUTDIR)\LobbySP.dll" /implib:"$(OUTDIR)\LobbySP.lib" /libpath:"..\lib" /libpath:"c:\WCEDreamcast\lib\retail" /libpath:"..\..\..\DrgaonARK\lib\SH4\wince\retail" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\LSPcallback.def"
LINK32_OBJS= \
	"$(INTDIR)\LSPdplay.obj" \
	"$(INTDIR)\LSPgroup.obj" \
	"$(INTDIR)\LSPmakelist.obj" \
	"$(INTDIR)\LSPplayer.obj" \
	"$(INTDIR)\LSPsession.obj"

"$(OUTDIR)\LobbySP.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("LSPcallback.dep")
!INCLUDE "LSPcallback.dep"
!ELSE 
!MESSAGE Warning: cannot find "LSPcallback.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "LSPcallback - Win32 (WCE SH4) Release" || "$(CFG)" == "LSPcallback - Win32 (WCE SH4) Debug"
SOURCE=.\LSPdplay.cpp

"$(INTDIR)\LSPdplay.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSPgroup.cpp

"$(INTDIR)\LSPgroup.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSPmakelist.cpp

"$(INTDIR)\LSPmakelist.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSPplayer.cpp

"$(INTDIR)\LSPplayer.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\LSPsession.cpp

"$(INTDIR)\LSPsession.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

