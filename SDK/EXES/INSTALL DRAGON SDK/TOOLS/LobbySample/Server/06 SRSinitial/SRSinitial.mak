# Microsoft Developer Studio Generated NMAKE File, Based on SRSinitial.dsp
!IF "$(CFG)" == ""
CFG=SRSinitial - Win32 Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� SRSinitial - Win32 Debug ��ݒ肵�܂��B
!ENDIF 

!IF "$(CFG)" != "SRSinitial - Win32 Release" && "$(CFG)" != "SRSinitial - Win32 Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "SRSinitial.mak" CFG="SRSinitial - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "SRSinitial - Win32 Release" ("Win32 (x86) Application" �p)
!MESSAGE "SRSinitial - Win32 Debug" ("Win32 (x86) Application" �p)
!MESSAGE 
!ERROR �����ȍ\�����w�肳��Ă��܂��B
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SRSinitial - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\module\SRSinitial.exe"


CLEAN :
	-@erase "$(INTDIR)\SRSinitial.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "..\module\SRSinitial.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SRSinitial.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRSinitial.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ..\SRSdebug\Release\SRSdebug.lib ..\SRSmanage\Release\SRSmanage.lib ..\SRSregistry\Release\SRSregistry.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\SRSinitial.pdb" /machine:I386 /out:"..\module/SRSinitial.exe" 
LINK32_OBJS= \
	"$(INTDIR)\SRSinitial.obj"

"..\module\SRSinitial.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRSinitial - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SRSinitial.exe"


CLEAN :
	-@erase "$(INTDIR)\SRSinitial.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SRSinitial.exe"
	-@erase "$(OUTDIR)\SRSinitial.ilk"
	-@erase "$(OUTDIR)\SRSinitial.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\SRSinitial.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRSinitial.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib ..\SRSdebug\Release\SRSdebug.lib ..\SRSmanage\Release\SRSmanage.lib ..\SRSregistry\Release\SRSregistry.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\SRSinitial.pdb" /debug /machine:I386 /out:"$(OUTDIR)\SRSinitial.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\SRSinitial.obj"

"$(OUTDIR)\SRSinitial.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SRSinitial.dep")
!INCLUDE "SRSinitial.dep"
!ELSE 
!MESSAGE Warning: cannot find "SRSinitial.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SRSinitial - Win32 Release" || "$(CFG)" == "SRSinitial - Win32 Debug"
SOURCE=.\SRSinitial.cpp

"$(INTDIR)\SRSinitial.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

