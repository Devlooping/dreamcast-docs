# Microsoft Developer Studio Generated NMAKE File, Based on SRSmanage.dsp
!IF "$(CFG)" == ""
CFG=SRSmanage - Win32 Debug
!MESSAGE �\�����w�肳��Ă��܂���B��̫�Ă� SRSmanage - Win32 Debug ��ݒ肵�܂��B
!ENDIF 

!IF "$(CFG)" != "SRSmanage - Win32 Release" && "$(CFG)" != "SRSmanage - Win32 Debug"
!MESSAGE �w�肳�ꂽ ����� Ӱ�� "$(CFG)" �͐���������܂���B
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "SRSmanage.mak" CFG="SRSmanage - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "SRSmanage - Win32 Release" ("Win32 (x86) Dynamic-Link Library" �p)
!MESSAGE "SRSmanage - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" �p)
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

!IF  "$(CFG)" == "SRSmanage - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\module\SRSmanage.dll"


CLEAN :
	-@erase "$(INTDIR)\SRSmanage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\SRSmanage.exp"
	-@erase "$(OUTDIR)\SRSmanage.lib"
	-@erase "..\module\SRSmanage.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SRSMANAGE_EXPORTS" /Fp"$(INTDIR)\SRSmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRSmanage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\SRSdebug\Release\SRSdebug.lib ..\SRSregistry\Release\SRSregistry.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\SRSmanage.pdb" /machine:I386 /def:".\SRSmanage.def" /out:"..\module/SRSmanage.dll" /implib:"$(OUTDIR)\SRSmanage.lib" 
DEF_FILE= \
	".\SRSmanage.def"
LINK32_OBJS= \
	"$(INTDIR)\SRSmanage.obj"

"..\module\SRSmanage.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SRSmanage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\SRSmanage.dll"


CLEAN :
	-@erase "$(INTDIR)\SRSmanage.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\SRSmanage.dll"
	-@erase "$(OUTDIR)\SRSmanage.exp"
	-@erase "$(OUTDIR)\SRSmanage.ilk"
	-@erase "$(OUTDIR)\SRSmanage.lib"
	-@erase "$(OUTDIR)\SRSmanage.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "SRSMANAGE_EXPORTS" /Fp"$(INTDIR)\SRSmanage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\SRSmanage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\SRSdebug\Release\SRSdebug.lib ..\SRSregistry\Release\SRSregistry.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\SRSmanage.pdb" /debug /machine:I386 /def:".\SRSmanage.def" /out:"$(OUTDIR)\SRSmanage.dll" /implib:"$(OUTDIR)\SRSmanage.lib" /pdbtype:sept 
DEF_FILE= \
	".\SRSmanage.def"
LINK32_OBJS= \
	"$(INTDIR)\SRSmanage.obj"

"$(OUTDIR)\SRSmanage.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("SRSmanage.dep")
!INCLUDE "SRSmanage.dep"
!ELSE 
!MESSAGE Warning: cannot find "SRSmanage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "SRSmanage - Win32 Release" || "$(CFG)" == "SRSmanage - Win32 Debug"
SOURCE=.\SRSmanage.cpp

"$(INTDIR)\SRSmanage.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

