# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Pix4 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Pix4 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Pix4 - Win32 Release" && "$(CFG)" != "Pix4 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pix4.mak" CFG="Pix4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pix4 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Pix4 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Pix4 - Win32 Debug"
MTL=mktyplib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pix4 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Pix4.exe"

CLEAN : 
	-@erase "$(INTDIR)\Board.obj"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\pix.obj"
	-@erase "$(INTDIR)\Pix4.res"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\Tile.obj"
	-@erase "$(OUTDIR)\Pix4.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Pix4.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Pix4.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pix4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Pix4.pdb" /machine:I386 /out:"$(OUTDIR)/Pix4.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Board.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\pix.obj" \
	"$(INTDIR)\Pix4.res" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\Tile.obj"

"$(OUTDIR)\Pix4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Pix4 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Pix4.exe"

CLEAN : 
	-@erase "$(INTDIR)\Board.obj"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\Draw.obj"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\pix.obj"
	-@erase "$(INTDIR)\Pix4.res"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\Tile.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Pix4.exe"
	-@erase "$(OUTDIR)\Pix4.ilk"
	-@erase "$(OUTDIR)\Pix4.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Pix4.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Pix4.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Pix4.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib\
 comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib\
 odbc32.lib odbccp32.lib ddraw.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Pix4.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Pix4.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Board.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\Draw.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\pix.obj" \
	"$(INTDIR)\Pix4.res" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\Tile.obj"

"$(OUTDIR)\Pix4.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Pix4 - Win32 Release"
# Name "Pix4 - Win32 Debug"

!IF  "$(CFG)" == "Pix4 - Win32 Release"

!ELSEIF  "$(CFG)" == "Pix4 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\pix.cpp
DEP_CPP_PIX_C=\
	".\board.h"\
	".\draw.h"\
	".\Exception.h"\
	".\pix.h"\
	".\sound.h"\
	".\square.h"\
	".\Tile.h"\
	

"$(INTDIR)\pix.obj" : $(SOURCE) $(DEP_CPP_PIX_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddutil.cpp
DEP_CPP_DDUTI=\
	".\ddutil.h"\
	

"$(INTDIR)\ddutil.obj" : $(SOURCE) $(DEP_CPP_DDUTI) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pix4.rc
DEP_RSC_PIX4_=\
	".\BEAM.BMP"\
	".\GROUND.BMP"\
	".\icon1.ico"\
	".\MAIN.BMP"\
	".\MESSAGE.BMP"\
	".\MIRROR.BMP"\
	".\NUKE.BMP"\
	".\REDBLOCK.BMP"\
	".\RUSTY.BMP"\
	".\STATIC.BMP"\
	".\TANK.BMP"\
	".\TEE.BMP"\
	".\WATER.BMP"\
	

"$(INTDIR)\Pix4.res" : $(SOURCE) $(DEP_RSC_PIX4_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Tile.cpp
DEP_CPP_TILE_=\
	".\board.h"\
	".\draw.h"\
	".\Exception.h"\
	".\pix.h"\
	".\sound.h"\
	".\square.h"\
	".\Tile.h"\
	

"$(INTDIR)\Tile.obj" : $(SOURCE) $(DEP_CPP_TILE_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Draw.cpp
DEP_CPP_DRAW_=\
	".\board.h"\
	".\ddutil.h"\
	".\draw.h"\
	".\Exception.h"\
	".\pix.h"\
	".\sound.h"\
	".\square.h"\
	".\Tile.h"\
	

"$(INTDIR)\Draw.obj" : $(SOURCE) $(DEP_CPP_DRAW_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Exception.cpp
DEP_CPP_EXCEP=\
	".\Exception.h"\
	

"$(INTDIR)\Exception.obj" : $(SOURCE) $(DEP_CPP_EXCEP) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Board.cpp
DEP_CPP_BOARD=\
	".\board.h"\
	".\draw.h"\
	".\Exception.h"\
	".\pix.h"\
	".\sound.h"\
	".\square.h"\
	".\Tile.h"\
	

"$(INTDIR)\Board.obj" : $(SOURCE) $(DEP_CPP_BOARD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\sound.cpp
DEP_CPP_SOUND=\
	".\pix.h"\
	".\sound.h"\
	

"$(INTDIR)\sound.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
