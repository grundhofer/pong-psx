@ECHO OFF
DEL *.ISO
DEL *.IMG
DEL *.TOC
CLS
ECHO Please Wait!
ECHO Building 7Pong.IMG...
BUILDCD -l -i7Pong.IMG 7Pong.CTI
REM PAUSE
ECHO Converting 7Pong.IMG to 7Pong.ISO...
STRIPISO S 2352 7Pong.IMG 7Pong.ISO
ECHO.
ECHO The GAME CD-ROM Image was Created Successfully!
PSXLICENSE /eu /i 7Pong.ISO