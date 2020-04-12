
cd ..

set var=0

:start
set /a var+=1
if %var% EQU 31 goto end

C:/Desktop/flashalizer/flashalizer/png2dbl.exe D:\universe\local\x\lotto\balls\%var%.png 22\b\b%var%.dbl

goto start

:end

cd 22

echo var has reached %var%.





