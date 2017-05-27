@echo off

REM Drop a font file on me and I'll install it!

set default_font_to_install="C:\PathTo\Some\Font.fon"
if "%~1"=="" ( set font_to_install=%DEFAULT_FONT_TO_INSTALL%) else (
	set font_to_install=%1
)
(
	echo Set objShell = CreateObject^("Shell.Application"^)
	echo Set objFolder = objShell.Namespace^(20^)
	echo objFolder.CopyHere "%FONT_TO_INSTALL:"=%"
) > "%TEMP%\tmp_%~n0.vbs"
WScript "%TEMP%\tmp_%~n0.vbs"
del "%TEMP%\tmp_%~n0.vbs"