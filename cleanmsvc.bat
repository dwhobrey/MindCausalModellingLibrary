@echo off
REM Batch script to clean Plato ready for archiving.

pushd Library
rmdir /S /Q .\Debug > junk.txt 2>&1
rmdir /S /Q .\Release > junk.txt 2>&1
del /F /Q /A *.user > junk.txt 2>&1
del /F /Q /A *.suo > junk.txt 2>&1
del /F /Q /A *.ncb > junk.txt 2>&1
del /F /Q junk.txt
popd

pushd Models
for /F %%i in ('dir /AD /B') do rmdir /S /Q .\%%i\Debug > junk.txt 2>&1
for /F %%i in ('dir /AD /B') do rmdir /S /Q .\%%i\Release > junk.txt 2>&1
for /F %%i in ('dir /AD /B') do del /F /S /Q /A .\%%i\*.user > junk.txt 2>&1
del /F /Q /A *.suo > junk.txt 2>&1
del /F /Q /A *.ncb > junk.txt 2>&1
del /F /Q junk.txt
popd

pushd Documentation
del /F /Q tmp\*.* > junk.txt 2>&1
rmdir /S /Q tmp\html > junk.txt 2>&1
del /F /Q junk.txt
popd

pushd Packages
for /F %%i in ('dir /AD /B') do rmdir /S /Q .\%%i\Debug > junk.txt 2>&1
for /F %%i in ('dir /AD /B') do rmdir /S /Q .\%%i\Release > junk.txt 2>&1
for /F %%i in ('dir /AD /B') do del /F /S /Q /A .\%%i\*.user > junk.txt 2>&1 
del /F /Q /A debug\*.* > junk.txt 2>&1
del /F /Q /A *.ncb > junk.txt 2>&1
del /F /Q /A *.suo > junk.txt 2>&1
del /F /Q junk.txt
popd

REM Miscellaneous files
del /F /Q /A debug\*.* > junk.txt 2>&1
del /F /Q /A *.ncb > junk.txt 2>&1
del /F /Q /A *.suo > junk.txt 2>&1
del /F /Q *.$$$ > junk.txt 2>&1
del /F /Q junk.txt

