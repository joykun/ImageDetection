@echo off

echo ----------------------------------------------------
echo Clear File
echo Press any key to delete all files with ending:
echo  *.aps *.idb *.ncp *.obj *.pch *.tmp *.sbr *.suo *.sdf *.ipch 
echo Visual c++/.Net junk 
echo ----------------------------------------------------
pause

del /F /Q /S *.aps *.idb *.ncp *.obj *.pch *.sbr *.pdb *.bsc *.ilk *.ncb *.opt *.dep *.sdf *.ipch 



pause


