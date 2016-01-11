@echo off
rem Script for archiving Plato and storing on ftp site.
rem Version: 0.1.0.0, 24th September 2008.
rem Cleans prior to archiving.
rem NOTE: The following environment variables: 
rem   PLATOPARENT=Absolute path to parent of project directory.
rem   FTPSite=URL of ftp site. 
rem   FTPSiteUser=ftp account name. 
rem   FTPSitePassword=ftp account password,
rem are expected to be set via the script platovars.bat.
rem This script requires lftp, tar, gzip and gpg.

rem This sets local environment variables.
if not exist platovars.bat goto end
call platovars
if %PLATOPARENT% == "" goto end

set GNUPGHOME=%PLATOPARENT%
set ArchiveName=Plato

rem Move to dir above Plato
cd %PLATOPARENT%

rem Work out name of prior and new archives.
set LastNumber="3"
set LastArchive="%ArchiveName%-0.tar.gz"
for /F "usebackq delims==" %%i in (`dir %ArchiveName%-*.tar.gz /OD /B`) do set LastArchive=%%i
set First=%LastArchive:~0,-7%
set ArchiveNumber=%First:~6%
if %ArchiveNumber% GTR %LastNumber% set LastNumber=%ArchiveNumber%
set /A NewNumber=1+%LastNumber%
set NewArchive=%ArchiveName%-%NewNumber%
set LastArchive=%ArchiveName%-%LastNumber%

rem Create archive file.
pushd Plato
call cleanplato.bat
popd
tar -cf %NewArchive%.tar Plato
gzip %NewArchive%.tar
gpg --passphrase %ArchivePassword% -c %NewArchive%.tar.gz

rem Copy archive file to ftp site.
lftp -d -c 'open -e "rm %LastArchive%.tar.gz.gpg" -u %FTPSiteUser%,%FTPSitePassword% %FTPSite%/ftp'
lftp -d -c 'open -e "put %NewArchive%.tar.gz.gpg" -u %FTPSiteUser%,%FTPSitePassword% %FTPSite%/ftp'

rem Tidy up.
rm %NewArchive%.tar.gz.gpg

:end
