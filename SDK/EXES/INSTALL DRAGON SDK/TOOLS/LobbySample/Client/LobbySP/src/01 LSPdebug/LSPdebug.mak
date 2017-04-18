# Microsoft Developer Studio Generated NMAKE File, Based on LSPdebug.dsp
!IF "$(CFG)" == ""
CFG=LSPdebug - Win32 (WCE SH4) Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� LSPdebug - Win32 (WCE SH4) Debug ��ݒ肵�܂��B
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

!IF "$(CFG)" != "LSPdebug - Win32 (WCE SH4) Release" && "$(CFG)" != "LSPdebug - Win32 (WCE SH4) Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "LSPdebug.mak" CFG="LSPdebug - Win32 (WCE SH4) Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "LSPdebug - Win32 (WCE SH4) Release" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
!MESSAGE "LSPdebug - Win32 (WCE SH4) Debug" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
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

!IF  "$(CFG)" == "LSPdebug - Win32 (WCE SH4) Release"

OUTDIR=.\WCESH4Rel
INTDIR=.\WCESH4Rel
# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

ALL : "$(OUTDIR)\LSPdebug.dll"


CLEAN :
	-@erase "$(INTDIR)\LSPdebug.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LSPdebug.dll"
	-@erase "$(OUTDIR)\LSPdebug.exp"
	-@erase "$(OUTDIR)\LSPdebug.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Ox /I "..\include" /I "c:\WCEDreamcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPdebug.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /Qgvp /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPdebug.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:no /pdb:"$(OUTDIR)\LSPdebug.pdb" /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\LSPdebug.def" /out:"$(OUTDIR)\LSPdebug.dll" /implib:"$(OUTDIR)\LSPdebug.lib" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\LSPdebug.def"
LINK32_OBJS= \
	"$(INTDIR)\LSPdebug.obj"

"$(OUTDIR)\LSPdebug.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\LSPdebug.dll"
   copy WCESH4Rel\LSPdebug.dll ..\dll
	copy WCESH4Rel\LSPdebug.lib ..\lib
	copy WCESH4Rel\LSPdebug.dll d:\DragonARK\release\katana\SH4\wince\retail
	copy WCESH4Rel\LSPdebug.dll c:\WCEDreamcast\release\retail
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "LSPdebug - Win32 (WCE SH4) Debug"

OUTDIR=.\WCESH4Dbg
INTDIR=.\WCESH4Dbg
# Begin Custom Macros
OutDir=.\WCESH4Dbg
# End Custom Macros

ALL : "$(OUTDIR)\LSPdebug.dll"


CLEAN :
	-@erase "$(INTDIR)\LSPdebug.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LSPdebug.dll"
	-@erase "$(OUTDIR)\LSPdebug.exp"
	-@erase "$(OUTDIR)\LSPdebug.ilk"
	-@erase "$(OUTDIR)\LSPdebug.lib"
	-@erase "$(OUTDIR)\LSPdebug.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Od /I "..\include" /I "c:\WCEDreamcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPdebug.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPdebug.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:yes /pdb:"$(OUTDIR)\LSPdebug.pdb" /debug /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\LSPdebug.def" /out:"$(OUTDIR)\LSPdebug.dll" /implib:"$(OUTDIR)\LSPdebug.lib" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\LSPdebug.def"
LINK32_OBJS= \
	"$(INTDIR)\LSPdebug.obj"

"$(OUTDIR)\LSPdebug.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("LSPdebug.dep")
!INCLUDE "LSPdebug.dep"
!ELSE 
!MESSAGE Warning: cannot find "LSPdebug.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "LSPdebug - Win32 (WCE SH4) Release" || "$(CFG)" == "LSPdebug - Win32 (WCE SH4) Debug"
SOURCE=.\LSPdebug.cpp

"$(INTDIR)\LSPdebug.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

