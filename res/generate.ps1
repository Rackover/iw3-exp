. "$PSScriptRoot/common.ps1"

# We base our iw4x.exe on iw4-159.exe
cp -Force "$PSScriptRoot\..\assets\iw3mp.exe" "$PSScriptRoot\iw3x.tmp"

# Remove certificate & build timestamp if any
& "$PSScriptRoot/PETool.exe" "$PSScriptRoot\iw3x.tmp" /unsign /destamp

# Apply new version info
echo "Updating version information..."
& "$PSScriptRoot/verpatch.exe" "$PSScriptRoot\iw3x.tmp" /rpdb /langid 0x409 /va 1.7.0.0 /pv 1.7.0.0 /s copyright "No rights reserved." /s desc "IW3xPort" /s title "iw3xport" /s product "IW3xPort" /s OriginalFilename "iw3xport.exe" /s pb 1

# Update the icon
echo "Updating the icon..."
winresourcer --operation=delete "--exeFile=$PSScriptRoot\iw3x.tmp" --resourceType=Icongroup --resourceName=1
winresourcer --operation=add "--exeFile=$PSScriptRoot\iw3x.tmp" --resourceType=Icongroup --resourceName=1 --lang=1033 "--resourceFile=$PSScriptRoot\..\assets\iw3.ico"

# Make it load iw4x.dll instead of steam_api.dll
# TODO - Replace with a more intelligent routine
echo "Updating the D3D9 library path..."
replace-string-keepoffset "$PSScriptRoot\iw3x.tmp" "d3d9.dll" "iw3x.dll"

# Let's place it into the root directory
mv -Force "$PSScriptRoot\iw3x.tmp" "$PSScriptRoot\..\iw3xport.exe"
