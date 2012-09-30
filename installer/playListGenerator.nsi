
;UAC.nsh must be in the include folder in the nsis install directory, and uac.dll in the plugins directory!
;=============================SETUP==========================================

!define PRODUCT_NAME "Ivar's Playlist Generator"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Ivar Eskerud Smith"
;!define PRODUCT_WEB_SITE "http://www.ivarsmith.no"  ;Comment out this line if you don`t want a website link
!define INSTALLER_NAME "${PRODUCT_NAME} ${PRODUCT_VERSION} setup.exe"
!define INSTALL_FOLDER_NAME "${PRODUCT_NAME}-${PRODUCT_VERSION}"
!define MAIN_EXECUTABLE "PlayListGenerator.exe"
!define RUN_ON_FINISH "PlayListGenerator.exe"
;!define LICENSE "lgpl.txt"
;!define PROMPT_DELETE_SETTINGS "settings.ini" 
;Comment out this line if the installer should not promt about keeping settings on uninstall. If not empty, the value should be the name of the settings file to keep/delete
;The programs folder folder in programfiles (INSTALL_FOLDER_NAME) will not be deleted if one wants to keep the settings

;=============================END OF SETUP====================================

;!addplugindir "C:\Program Files\NSIS\Plugins"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_EXECUTABLE}"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

!addincludedir "./"
!addplugindir "./"
!include UAC.nsh

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!ifdef LICENSE
	!insertmacro MUI_PAGE_LICENSE ${LICENSE}
!endif
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "${INSTALL_FOLDER_NAME}"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN
!define MUI_FINISHPAGE_RUN_FUNCTION FinishRun
!insertmacro MUI_PAGE_FINISH
;!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_EXECUTABLE}" ; Finish page


; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

RequestExecutionLevel user    /* RequestExecutionLevel REQUIRED! */

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${INSTALLER_NAME}"
InstallDir "$PROGRAMFILES\${INSTALL_FOLDER_NAME}"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "../release\PlayListGenerator.exe"  
  File "..\libtag.dll"
  File "..\example.js"
  File "..\zlib1.dll"
  File "C:\MinGW\bin\libstdc++-6.dll"
  File "C:\MinGW\bin\libgcc_s_dw2-1.dll"
  File "C:\MinGW\bin\mingwm10.dll"
  File "../release\beautify.js"
  File $%QTDIR%\bin\QtCore4.dll
  File $%QTDIR%\bin\QtGui4.dll
  File $%QTDIR%\bin\QtScript4.dll
  File $%QTDIR%\bin\QtScriptTools4.dll
  
  SetOutPath "$INSTDIR" ;the application will be started in the last outpath that is set!
  

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk" "$INSTDIR\${MAIN_EXECUTABLE}"
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}-${PRODUCT_VERSION}.lnk" "$INSTDIR\${MAIN_EXECUTABLE}"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  !ifdef PRODUCT_WEB_SITE
	  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
	  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME} website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  !endif
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\${MAIN_EXECUTABLE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${MAIN_EXECUTABLE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  !ifdef PRODUCT_WEB_SITE
	WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  !endif
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

; Attempt to give the UAC plug-in a user process and an admin process.
Function .OnInit


UAC_tryagain:
        !insertmacro UAC_RunElevated
       ${Switch} $0
        ${Case} 0
        	${IfThen} $1 = 1 ${|} Quit ${|}
        	${IfThen} $3 <> 0 ${|} ${Break} ${|}
        	${If} $1 = 3
        		MessageBox MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND "Administrative rights required to continue, aborting." /SD IDNO IDOK UAC_tryagain IDNO 0
       	${EndIf}
        ${Case} 1223
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Administrative rights required to continue, aborting."
        	Quit
        ${Case} 1062
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Logon service not running, aborting!"
        	Quit
        ${Default}
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Unable to elevate, error $0"
        	Quit
        ${EndSwitch}
 

 
FunctionEnd

Function FinishRun
	!insertmacro UAC_AsUser_ExecShell "" "$INSTDIR\${RUN_ON_FINISH}" "" "" ""
FunctionEnd

Function .OnInstFailed
    ;UAC::Unload ;Must call unload!
FunctionEnd
 
Function un.onUninstSuccess
  ;UAC.Unload ;Must call unload!
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer. Note that your license and setting files might be left in the installation folder."
FunctionEnd

Function un.onInit
	
	UAC_tryagain:
        !insertmacro UAC_RunElevated
        ${Switch} $0
        ${Case} 0
        	${IfThen} $1 = 1 ${|} Quit ${|}
        	${IfThen} $3 <> 0 ${|} ${Break} ${|}
        	${If} $1 = 3
        		MessageBox MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND "Administrative rights required to continue, aborting." /SD IDNO IDOK UAC_tryagain IDNO 0
        	${EndIf}
        ${Case} 1223
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Administrative rights required to continue, aborting."
        	Quit
        ${Case} 1062
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Logon service not running, aborting!"
        	Quit
        ${Default}
        	MessageBox MB_ICONSTOP|MB_TOPMOST|MB_SETFOREGROUND "Unable to elevate, error $0"
        	Quit
        ${EndSwitch}
	
	MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
    Abort	
	

FunctionEnd

Section Uninstall
	; close program if running
		;FindWindow $0 "RainmeterTrayClass"
		;${If} $0 != "0"
		;	Exec '"$INSTDIR\Rainmeter.exe" !RainmeterQuit'
;
;			; wait up to for up to 5 seconds for Rainmeter to close
;			StrCpy $1 "0"
;			${DoWhile} ${ProcessExists} "Rainmeter.exe"
;				IntOp $1 $1 + 1
;				${If} $1 >= "10"
;					MessageBox MB_RETRYCANCEL|MB_ICONSTOP "Error: Failed to close Rainmeter.$\n$\nPlease close Rainmeter manually and try again." IDRETRY +2
;					Quit
;				${EndIf}
;				Sleep 500
;			${Loop}
;		${EndIf}

  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\iRes.exe"
  Delete "$INSTDIR\PlayListGenerator.exe"
  Delete "$INSTDIR\example.js"
  Delete "$INSTDIR\libtag.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\beautify.js"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtScript4.dll"
  Delete "$INSTDIR\QtScriptTools4.dll"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME} website.lnk"
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\${PRODUCT_NAME}.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR"  

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd