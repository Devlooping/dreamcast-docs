# Microsoft Developer Studio Generated NMAKE File, Based on LSPmanage.dsp
!IF "$(CFG)" == ""
CFG=LSPmanage - Win32 (WCE SH4) Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� LSPmanage - Win32 (WCE SH4) Debug ��ݒ肵�܂��B
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

!IF "$(CFG)" != "LSPmanage - Win32 (WCE SH4) Release" && "$(CFG)" != "LSPmanage - Win32 (WCE SH4) Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "LSPmanage.mak" CFG="LSPmanage - Win32 (WCE SH4) Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "LSPmanage - Win32 (WCE SH4) Release" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
!MESSAGE "LSPmanage - Win32 (WCE SH4) Debug" ("Win32 (WCE SH4) Dynamic-Link Library" �p)
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

!IF  "$(CFG)" == "LSPmanage - Win32 (WCE SH4) Release"

OUTDIR=.\WCESH4Rel
INTDIR=.\WCESH4Rel
# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

ALL : "$(OUTDIR)\LSPmanage.dll"


CLEAN :
	-@erase "$(INTDIR)\SRLmanage.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LSPmanage.dll"
	-@erase "$(OUTDIR)\LSPmanage.exp"
	-@erase "$(OUTDIR)\LSPmanage.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Ox /I "..\include" /I "c:\WCEDreamcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /Qgvp /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPmanage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib ..\lib\LSPdebug.lib ..\lib\LSPregistry.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:no /pdb:"$(OUTDIR)\LSPmanage.pdb" /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\SRLmanage.def" /out:"$(OUTDIR)\LSPmanage.dll" /implib:"$(OUTDIR)\LSPmanage.lib" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\SRLmanage.def"
LINK32_OBJS= \
	"$(INTDIR)\SRLmanage.obj"

"$(OUTDIR)\LSPmanage.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\LSPmanage.dll"
   copy WCESH4Rel\LSPmanage.dll ..\dll
	copy WCESH4Rel\LSPmanage.lib ..\lib
	copy WCESH4Rel\LSPmanage.dll d:\DragonARK\release\katana\SH4\wince\retail
	copy WCESH4Rel\LSPmanage.dll c:\WCEDreamcast\release\retail
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "LSPmanage - Win32 (WCE SH4) Debug"

OUTDIR=.\WCESH4Dbg
INTDIR=.\WCESH4Dbg
# Begin Custom Macros
OutDir=.\WCESH4Dbg
# End Custom Macros

ALL : "$(OUTDIR)\LSPmanage.dll"


CLEAN :
	-@erase "$(INTDIR)\SRLmanage.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\LSPmanage.dll"
	-@erase "$(OUTDIR)\LSPmanage.exp"
	-@erase "$(OUTDIR)\LSPmanage.ilk"
	-@erase "$(OUTDIR)\LSPmanage.lib"
	-@erase "$(OUTDIR)\LSPmanage.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Od /I "..\include" /I "c:\WCEDreamcast\inc" /I "..\..\..\DragonARK\inc\oak" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /Fp"$(INTDIR)\LSPmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\LSPmanage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib ..\lib\LSPdebug.lib ..\lib\LSPregistry.lib /nologo /entry:"_DllMainCRTStartup" /base:"0x10000000" /align:"4096" /dll /incremental:yes /pdb:"$(OUTDIR)\LSPmanage.pdb" /debug /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /def:".\SRLmanage.def" /out:"$(OUTDIR)\LSPmanage.dll" /implib:"$(OUTDIR)\LSPmanage.lib" /subsystem:$(CESubsystem) /STACK:65536,4096 
DEF_FILE= \
	".\SRLmanage.def"
LINK32_OBJS= \
	"$(INTDIR)\SRLmanage.obj"

"$(OUTDIR)\LSPmanage.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("LSPmanage.dep")
!INCLUDE "LSPmanage.dep"
!ELSE 
!MESSAGE Warning: cannot find "LSPmanage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "LSPmanage - Win32 (WCE SH4) Release" || "$(CFG)" == "LSPmanage - Win32 (WCE SH4) Debug"
SOURCE=.\SRLmanage.cpp

"$(INTDIR)\SRLmanage.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

