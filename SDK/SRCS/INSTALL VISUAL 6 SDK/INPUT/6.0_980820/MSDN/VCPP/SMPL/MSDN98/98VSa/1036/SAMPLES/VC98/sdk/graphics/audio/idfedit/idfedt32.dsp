# Microsoft Developer Studio Project File - Name="idfedt32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=idfedt32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "idfedt32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "idfedt32.mak" CFG="idfedt32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "idfedt32 - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "idfedt32 - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "idfedt32 - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "idfedt32.exe"
# PROP Bsc_Name "idfedt32.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "idfedt32 - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f makefile"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f makefile"
# PROP Rebuild_Opt "/a"
# PROP Target_File "idfedt32.exe"
# PROP Bsc_Name "idfedt32.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "idfedt32 - Win32 Release"
# Name "idfedt32 - Win32 Debug"

!IF  "$(CFG)" == "idfedt32 - Win32 Release"

!ELSEIF  "$(CFG)" == "idfedt32 - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=.\MAKEFILE
# End Source File
# Begin Group "Source Files" 
# Begin Source File

SOURCE=.\DRAWSTR.C
# End Source File
# Begin Source File

SOURCE=.\HEAD.C
# End Source File
# Begin Source File

SOURCE=.\IDF.C
# End Source File
# Begin Source File

SOURCE=.\MAIN.C
# End Source File
# Begin Source File

SOURCE=.\TRIDEE.C
# End Source File
# Begin Source File

SOURCE=.\IDFEDIT.DEF
# End Source File
# End Group 
# Begin Group "Header Files" 
# Begin Source File

SOURCE=.\DRAWSTR.H
# End Source File
# Begin Source File

SOURCE=.\GLOBALS.H
# End Source File
# Begin Source File

SOURCE=.\IDFEDIT.H
# End Source File
# Begin Source File

SOURCE=.\OLDIDF.H
# End Source File
# Begin Source File

SOURCE=.\RES.H
# End Source File
# Begin Source File

SOURCE=.\TRIDEE.H
# End Source File
# End Group 
# Begin Group "Resource Files" 
# Begin Source File

SOURCE=.\IDFEDIT.RC
# End Source File
# Begin Source File

SOURCE=.\IDF.ICO
# End Source File
# Begin Source File

SOURCE=.\PIANO.ICO
# End Source File
# End Group 
# End Target
# End Project
