REM do rootcint for MC ROOT classes

REM Save the current directory, so we can return when finished
cd > dirstor.txt
copy %MCROOTDATAROOT%\cmt\cd.cmd+dirstor.txt %MCROOTDATAROOT%\changeDir.bat
del dirstor.txt

cd %MCROOTDATAROOT%\mcRootData
REM Remove old cint files
del mcRootDataCint.cxx
del mcRootDataCint.h

%ROOT_PATH%\bin\rootcint mcRootDataCint.cxx -c McVertex.h McParticle.h McEvent.h mcRootDataLinkDef.h

REM Return to the original directory
call %MCROOTDATAROOT%\changeDir.bat
del %MCROOTDATAROOT%\changeDir.bat
