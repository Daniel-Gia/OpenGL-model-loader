[Setup]
AppName=OpenGL Model Loader
AppVersion=0.1.1
AppPublisher=Daniel Giacobelli

DefaultDirName={autopf}\OpenGLModelLoader
DefaultGroupName=OpenGL Model Loader
UninstallDisplayIcon={app}\OpenGL-Model-Loader.exe
Compression=lzma2
SolidCompression=yes
OutputDir=installer
OutputBaseFilename=OpenGL-Model-Loader-Installer
LicenseFile=LICENSE

[Files]
Source: "final_build\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\OpenGL Model Loader"; Filename: "{app}\OpenGL-Model-Loader.exe"
Name: "{autodesktop}\OpenGL Model Loader"; Filename: "{app}\OpenGL-Model-Loader.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
