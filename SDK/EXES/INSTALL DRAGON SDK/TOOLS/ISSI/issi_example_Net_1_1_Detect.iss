
; ISSI EXAMPLE SCRIPT
;
; LOGAN's Inno Setup Script Includes (ISSI)
; http://www.albartus.com/issi/
;
; (c) 2005 Jan Albartus / LOGAN Interactive

; This example shows you how to detect Microsoft .NET Framework v1.1

; Set inclusion for languages:
#define ISSI_English
#define ISSI_Dutch
#define ISSI_German
; Detect MicroSoft .NET Framework:
#define ISSI_NetDetect
; Include ISSI:
#define ISSI_IncludePath "C:\ISSI"
#include ISSI_IncludePath+"\_issi.isi"

[Setup]
AppName=ISSI Example .NET 1.1 detect
AppVerName=ISSI Example v1.0
AppVersion=1.0
AppPublisher=LOGAN Interactive
AppPublisherURL=http://www.albartus.com
AppSupportURL=http://www.albartus.com
AppUpdatesURL=http://www.albartus.com
AppCopyright=Jan Albartus
CreateAppDir=no
CreateUninstallRegKey=no
Uninstallable=no
outputBaseFileName=ISSI_Net_1_1_Detect
