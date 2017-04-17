# Microsoft Developer Studio Generated NMAKE File, Based on testrun.dsp
!IF "$(CFG)" == ""
CFG=testrun - Win32 (WCE SH4) Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� testrun - Win32 (WCE SH4) Debug ��ݒ肵�܂��B
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

!IF "$(CFG)" != "testrun - Win32 (WCE SH4) Release" && "$(CFG)" != "testrun - Win32 (WCE SH4) Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "testrun.mak" CFG="testrun - Win32 (WCE SH4) Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "testrun - Win32 (WCE SH4) Release" ("Win32 (WCE SH4) Application" �p)
!MESSAGE "testrun - Win32 (WCE SH4) Debug" ("Win32 (WCE SH4) Application" �p)
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

!IF  "$(CFG)" == "testrun - Win32 (WCE SH4) Release"

OUTDIR=.\WCESH4Rel
INTDIR=.\WCESH4Rel
# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

ALL : "$(OUTDIR)\testrun.exe"


CLEAN :
	-@erase "$(INTDIR)\testrun.obj"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\testrun.exe"
	-@erase "$(OUTDIR)\testrun.ilk"
	-@erase "$(OUTDIR)\testrun.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Ox /I "..\lobbySP\inc" /I "c:\WCEDreamcast\inc" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "NDEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_UNICODE" /D "DRAGON" /D "STRICT" /Fp"$(INTDIR)\testrun.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /Qgvp /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\testrun.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=coredll.lib dplayx.lib dxguid.lib uuid.lib mras.lib platutil.lib /nologo /entry:"WinMainCRTStartup" /base:"0x00010000" /align:"4096" /incremental:yes /pdb:"$(OUTDIR)\testrun.pdb" /debug /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /out:"$(OUTDIR)\testrun.exe" /libpath:"..\LobbySP\lib" /libpath:"C:\WCEDreamcast\lib\retail" /libpath:"..\..\..\DragonARK\lib\SH4\wince\retail" /subsystem:$(CESubsystem) /STACK:65536,4096 
LINK32_OBJS= \
	"$(INTDIR)\testrun.obj"

"$(OUTDIR)\testrun.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\WCESH4Rel
# End Custom Macros

$(DS_POSTBUILD_DEP) : "$(OUTDIR)\testrun.exe"
   copy WCESH4Rel\testrun.exe d:\DragonARK\release\katana\SH4\wince\retail
	copy WCESH4Rel\testrun.exe c:\WCEDreamcast\release\retail
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "testrun - Win32 (WCE SH4) Debug"

OUTDIR=.\WCESH4Dbg
INTDIR=.\WCESH4Dbg
# Begin Custom Macros
OutDir=.\WCESH4Dbg
# End Custom Macros

ALL : "$(OUTDIR)\testrun.exe" "$(OUTDIR)\testrun.bsc"


CLEAN :
	-@erase "$(INTDIR)\testrun.obj"
	-@erase "$(INTDIR)\testrun.sbr"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\testrun.bsc"
	-@erase "$(OUTDIR)\testrun.exe"
	-@erase "$(OUTDIR)\testrun.ilk"
	-@erase "$(OUTDIR)\testrun.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MC /W3 /Zi /Od /I "..\lobbySP\inc" /I "c:\WCEDreamcast\inc" /D _WIN32_WCE=$(CEVersion) /D "$(CEConfigName)" /D "DEBUG" /D "SHx" /D "SH4" /D "_SH4_" /D UNDER_CE=$(CEVersion) /D "UNICODE" /D "_MBCS" /D "_UNICODE" /D "DRAGON" /D "STRICT" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\testrun.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /Qsh4r7 /Qs /Qfast /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\testrun.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\testrun.sbr"

"$(OUTDIR)\testrun.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=coredll.lib dplayx.lib dxguid.lib uuid.lib mras.lib platutil.lib /nologo /entry:"WinMainCRTStartup" /base:"0x00010000" /align:"4096" /incremental:yes /pdb:"$(OUTDIR)\testrun.pdb" /debug /machine:SH4 /nodefaultlib:"$(CENoDefaultLib)" /out:"$(OUTDIR)\testrun.exe" /libpath:"..\LobbySP\lib" /libpath:"C:\WCEDreamcast\lib\retail" /libpath:"..\..\..\DragonARK\lib\SH4\wince\retail" /subsystem:$(CESubsystem) /STACK:65536,4096 
LINK32_OBJS= \
	"$(INTDIR)\testrun.obj"

"$(OUTDIR)\testrun.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("testrun.dep")
!INCLUDE "testrun.dep"
!ELSE 
!MESSAGE Warning: cannot find "testrun.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "testrun - Win32 (WCE SH4) Release" || "$(CFG)" == "testrun - Win32 (WCE SH4) Debug"
SOURCE=.\testrun.cpp

!IF  "$(CFG)" == "testrun - Win32 (WCE SH4) Release"


"$(INTDIR)\testrun.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "testrun - Win32 (WCE SH4) Debug"


"$(INTDIR)\testrun.obj"	"$(INTDIR)\testrun.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

