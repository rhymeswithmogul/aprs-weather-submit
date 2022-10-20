@ECHO OFF
REM aprs-weather-submit
REM Copyright (c) 2019-2022 Colin Cogle (colin@colincogle.name)
REM
REM This file, make-dos.bat, is part of aprs-weather-submit.
REM (https://github.com/rhymeswithmogul/aprs-weather-submit)
REM
REM This program is free software: you can redistribute it and/or modify it under
REM the terms of the GNU Affero General Public License as published by the Free
REM Software Foundation, either version 3 of the License, or (at your option) any
REM later version.
REM
REM This program is distributed in the hope that it will be useful, but WITHOUT
REM ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
REM FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
REM details.
REM
REM You should have received a copy of the GNU Affero General Public License along
REM with this program.  If not, see (https://www.gnu.org/licenses/agpl-3.0.html).


REM Determine best command line flags.
REM There will be different sets for "make" and "make debug".
SET OWCCARGS=-oaprswx.exe -std=c99 -march=i86 -mcmodel=s -bdos -D_DOS=1 -Icontrib\freege~1.11

IF "%1" == "DEBUG"
GOTO DEBUG

:RELEASE
SET OWCCARGS=%OWCCARGS% -Os -s -P
GOTO BUILD

:DEBUG
SET DEBUG=yes
SET OWCCARGS=%OWCCARGS% -O0 -g3 -DDEBUG -Wall -Wextra -Werror -fbrowser -fr -fmAPRSWX.MAP


:BUILD
ECHO *** Building FreeGetopt
CD CONTRIB\FREEGE~1.11
OWCC -c %OWCCARGS% getopt.c
ECHO *** Done
CD ..\..

ECHO *** Building main app
CD SRC
OWCC %OWCCARGS% c99math.c help.c aprs-is.c aprs-wx.c main.c
ECHO ** Done

MOVE /Y /V APRSWX.EXE ..\..
IF %DEBUG%=="yes" (MOVE /Y /V APRSWX.MAP ..\..)
CD ..\..
ECHO *** Done!

IF EXIST APRSWX.EXE
	(ECHO *** Build successful!  Run APRSWX.EXE.)
ELSE
	(ECHO *** Build unsuccessful.)