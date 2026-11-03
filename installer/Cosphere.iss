#define AppName "Cosphere"
#define AppVersion "1.0.0"
[Setup]
AppId={{B08AA3B0-41B6-4B33-8596-5C9B630CA3AC}
AppName={#AppName}
AppVersion={#AppVersion}
AppPublisher=Danilo Pereira
DefaultDirName={commoncf64}\VST3
DisableDirPage=yes
DisableProgramGroupPage=yes
UninstallDisplayName=Cosphere VST3
OutputDir=..\dist
OutputBaseFilename=Cosphere-1.0.0-Windows-x64-Setup
Compression=lzma2
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
PrivilegesRequired=admin
WizardStyle=modern
LicenseFile=..\LICENSE

[Files]
Source: "..\build\Cosphere_artefacts\Release\VST3\Cosphere.vst3\*"; DestDir: "{app}\Cosphere.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

#if FileExists("..\Assets\Redaction-LICENSE.txt")
Source: "..\Assets\Redaction-LICENSE.txt"; DestDir: "{app}\Cosphere.vst3"; DestName: "Redaction-LICENSE.txt"; Flags: ignoreversion
#endif
Source: "..\LICENSE"; DestDir: "{app}\Cosphere.vst3"; DestName: "Cosphere-LICENSE.txt"; Flags: ignoreversion

[Icons]
; VST3 is loaded by a DAW; no desktop icon needed.

[UninstallDelete]
Type: filesandordirs; Name: "{app}\Cosphere.vst3"
