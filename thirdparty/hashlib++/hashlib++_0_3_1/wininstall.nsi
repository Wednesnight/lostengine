; 
; hashlib++ - a simple hash library for C++
; 
; Copyright (c) 2007,2008 Benjamin Grüdelbach
; 
; Redistribution and use in source and binary forms, with or without modification,
; are permitted provided that the following conditions are met:
; 
; 	1)     Redistributions of source code must retain the above copyright
; 	       notice, this list of conditions and the following disclaimer.
; 
; 	2)     Redistributions in binary form must reproduce the above copyright
; 	       notice, this list of conditions and the following disclaimer in
; 	       the documentation and/or other materials provided with the
; 	       distribution.
; 	     
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
; ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;
;--------------------------------------------------------------------------------
; 
; This script creates a windows installer "Setup.exe" 
; for hashlib++. 
;
; Benjamin Grüdelbach
; Di 16 Okt 2007
; 
;--------------------------------

  ;Include Modern UI
  !include "MUI.nsh"

;--------------------------------

  ; The name of the installer
  Name "hashlib++"

  ; The file to write
  OutFile "Setup.exe"

  ; The default installation directory
  InstallDir $PROGRAMFILES\hashlib++

  ; Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\hashlib++" ""

;--------------------------------

  ; Interface Settings
  !define MUI_ABORTWARNING

;--------------------------------

  ; Pages
  !insertmacro MUI_PAGE_LICENSE "../doc/LICENSE.TXT"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------

  ; Languages
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------

 ; The stuff to install
 ; Name is not important cause there is 
 ; no "component" page
 Section "" 

	  ; Set output path to the installation directory.
	  SetOutPath $INSTDIR
	  
	  ; Files to install:
	  File hl_hashwrapper.h
	  File hl_exception.h
	  File hl_md5.h
	  File hl_md5wrapper.h
	  File hl_sha1.h
	  File hl_sha1wrapper.h
	  File hl_sha256.h
	  File hl_sha256wrapper.h
	  File hl_sha2mac.h
	  File hl_sha2ext.h
	  File hl_sha384wrapper.h
	  File hl_sha512wrapper.h

	  ; Put hashlib++ as a static lib in this dir
	  ; and name it "hl++.lib"
	  File hl++.lib
	  ; make sure this file has been build using 
          ; vim's "TOhtml" command
	  File "..\doc\README.TXT.html"
	  
	  ; Store installation folder
	  WriteRegStr HKLM "Software\hashlib++" "" $INSTDIR
	  
	  ; Create uninstaller
	  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hashlib++" "DisplayName" "hashlib++"
	  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hashlib++" "UninstallString" '"$INSTDIR\uninstall.exe"'
	  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hashlib++" "NoModify" 1
	  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hashlib++" "NoRepair" 1
	  WriteUninstaller "$INSTDIR\Uninstall.exe"

 SectionEnd 

;--------------------------------
; Uninstaller Section

Section "Uninstall"

	  ; Delete files
	  Delete "$INSTDIR\Uninstall.exe"
	  Delete "$INSTDIR\hl++.lib"
	  Delete "$INSTDIR\hl_hashwrapper.h"
	  Delete "$INSTDIR\hl_exception.h"
	  Delete "$INSTDIR\hl_md5.h"
	  Delete "$INSTDIR\hl_md5wrapper.h"
	  Delete "$INSTDIR\hl_sha1.h"
	  Delete "$INSTDIR\hl_sha1wrapper.h"
	  Delete "$INSTDIR\hl_sha256.h"
	  Delete "$INSTDIR\hl_sha256wrapper.h"
	  Delete "$INSTDIR\README.TXT.html"
	  Delete "$INSTDIR\hl_sha2mac.h"
	  Delete "$INSTDIR\hl_sha2ext.h"
	  Delete "$INSTDIR\hl_sha384wrapper.h"
	  Delete "$INSTDIR\hl_sha512wrapper.h"

	  ; Remove the directory
	  RMDir "$INSTDIR"

	  ; delete reg-keys
	  DeleteRegKey /ifempty HKLM "Software\hashlib++"
	  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hashlib++"

SectionEnd

;--------------------------------
;EOF
