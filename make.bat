@ECHO OFF
REM aprs-weather-submit
REM Copyright (c) 2019-2022 Colin Cogle (colin@colincogle.name)
REM
REM This file, make-dos.bat, is part of aprs-weather-submit.
REM (https://github.com/rhymeswithmogul/aprs-weather-submit)
REM
REM This program is free software: you can redistribute it and/or modify
REM it under the terms of the GNU Affero General Public License as pub-
REM lished by the  Free Software Foundation,  either  version 3  of the
REM License, or (at your option) any later version.
REM
REM This program is distributed in the hope that it will be useful, but
REM WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANT-
REM ABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
REM General Public License for more details.
REM
REM You should have received a copy of the GNU Affero General Public
REM License along with this program.  If not, see:
REM http://www.gnu.org/licenses/agpl-3.0.html.

REM Start processing command-line modes.
REM If none are specified, we'll fall through to the RELEASE modes.
REM Yes, they are case sensitive.
IF "%1"=="debug" GOTO DEBUG
IF "%1"=="DEBUG" GOTO DEBUG
IF "%1"=="clean" GOTO CLEAN
IF "%1"=="CLEAN" GOTO CLEAN
IF "%1"=="/?"    GOTO HELP
IF "%1"=="help"  GOTO HELP
IF "%1"=="HELP"  GOTO HELP
GOTO RELEASE

REM Help.
ECHO This batch file will build aprs-weather-submit (as APRSWX.EXE).  Without any
ECHO options, an optimized build for all CPU types will be created.  However, you
ECHO can create builds intended for a certain CPU-/FPU-compatible combination:
ECHO
ECHO	(none)	Any CPU, no FPU
ECHO	287		Any CPU, 287 FPU
ECHO	386		386 CPU, no FPU
ECHO	386287	386 CPU, 287 FPU
ECHO	387		386 CPU, 387 FPU
ECHO	486SX	486 CPU, no FPU
ECHO	486		486 CPU with FPU
ECHO	586		586 CPU (includes Pentium)
ECHO	686		686 CPU (includes Pentium Pro and Pentium II)
ECHO
ECHO There are also other modes available besides the default RELEASE build:
ECHO	CLEAN	Delete all intermediary and output files.
ECHO	DEBUG	Build this app with debugging output.
ECHO
GOTO END

REM Clean up after previous runs.  Not all of these files may exist.
:CLEAN
DEL /-P APRSWX.EXE APRSWX.MAP CONTRIB\FREEGE~1.11\*.O CONTRIB\FREEGE~1.11\*.MBR CONTRIB\FREEGE~1.11\APRSWX.MAP CONTRIB\FREEGE~1.11\APRSWX.EXE
GOTO END

REM Prepare for a debug build.
:DEBUG
ECHO *** Preparing environment to build aprs-weather-submit (debug build)
SET OWCCARGS=-O0 -g3 -DDEBUG -Wall -Wextra -fmAPRSWX.MAP
GOTO CONFIGURE

REM Prepare for a normal build.
:RELEASE
ECHO *** Preparing environment to build aprs-weather-submit
SET OWCCARGS=-Os -s
GOTO CONFIGURE


REM Start of configure phase.  Take our environment and verify that
REM everything we need is there.
:CONFIGURE
ECHO *** Checking for OWCC.EXE
IF EXIST %WATCOM%\BINW\OWCC.EXE GOTO CONFIGURE2
ECHO OWCC.EXE not found.  Try running OWSETENV.BAT.
GOTO END
:CONFIGURE2
ECHO OWCC.EXE found at %WATCOM%\BINW.

ECHO *** Setting up environment
ECHO *** Checking for user-requested CPU optimizations.
SET OWCCARGS=%OWCCARGS% -oaprswx.exe -std=c99 -bdos -D_DOS=1 -mcmodel=t
IF "%1"=="287"    GOTO CONFIGURE287
IF "%1"=="386"    GOTO CONFIGURE386
IF "%1"=="386287" GOTO CONFIGURE386287
IF "%1"=="387"    GOTO CONFIGURE387
IF "%1"=="486"    GOTO CONFIGURE486
IF "%1"=="486SX"  GOTO CONFIGURE486NOFPU
IF "%1"=="586"    GOTO CONFIGURE586
IF "%1"=="686"    GOTO CONFIGURE686
ECHO *** Not optimizing for any particular CPU
SET OWCCARGS=%OWCCARGS% -msoft-float
GOTO BUILD

:CONFIGURE287
ECHO *** Optimizing for an 80286-compatible CPU and 80287-compatible FPU
SET OWCCARGS=%OWCCARGS% -fpmath=287
GOTO BUILD

:CONFIGURE386
ECHO *** Optimizing for an 80386-compatible CPU
SET OWCCARGS=%OWCCARGS% -mtune=386 -msoft-float
GOTO BUILD

:CONFIGURE386287
ECHO *** Optimizing for an 80386-compatible CPU and 80287-compatible FPU
SET OWCCARGS=%OWCCARGS% -mtune=386 -fpmath=287
GOTO BUILD

:CONFIGURE387
ECHO *** Optimizing for an 80386-compatible CPU and 80387-compatible FPU
SET OWCCARGS=%OWCCARGS% -mtune=386 -fpmath=387
GOTO BUILD

:CONFIGURE486
ECHO *** Optimizing for an 80486-compatible CPU
SET OWCCARGS=%OWCCARGS% -mtune=486 -fpmath=387
GOTO BUILD

:CONFIGURE486NOFPU
ECHO *** Optimizing for a 486SX CPU
SET OWCCARGS=%OWCCARGS% -mtune=486 -msoft-float
GOTO BUILD

:CONFIGURE586
ECHO *** Optimizing for a Pentium/5x86-compatible CPU
SET OWCCARGS=%OWCCARGS% -mtune=586 -fptune=586
GOTO BUILD

:CONFIGURE686
ECHO *** Optimizing for a Pentium II/6x86-compatible CPU
SET OWCCARGS=%OWCCARGS% -mtune=686 -fptune=686
GOTO BUILD

REM End of configure phase.
REM Start build phae.

:BUILD
ECHO *** Building main app

REM Due to limits on the maximum command line length, we are changing
REM into the longest path and building from there.  If anyone has a
REM better idea, my DOS skills are rusty.
CD contrib\freege~1.11
OWCC %OWCCARGS% getopt.h getopt.c ..\..\src\*.h ..\..\src\*.c
IF NOT ERRORLEVEL 0 GOTO END
ECHO *** Done
IF EXIST APRSWX.EXE GOTO BUILDOK

REM If the build did not succeed:
ECHO Build unsuccessful.
CD ..\..
GOTO END

REM If the build succeeded:
:BUILDOK
MOVE APRSWX.EXE ..\..
IF "%1"=="debug" MOVE APRSWX.MAP ..\..
IF "%1"=="DEBUG" MOVE APRSWX.MAP ..\..
ECHO Build successful!  Run APRSWX.EXE.
CD ..\..
GOTO END

REM Clean up after our previous passes.  We stuff the environment with
REM data, which can cause errors for the user later.
:END
SET CFILES=
SET OWCCARGS=
