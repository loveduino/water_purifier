@echo off

if exist C:\Keil\ARM\BIN40\fromelf.exe (
if exist .\..\MDK-ARM\STM320518_EVAL\STM320518_EVAL.axf (C:\Keil\ARM\BIN40\fromelf.exe ".\..\MDK-ARM\STM320518_EVAL\STM320518_EVAL.axf" --bin --output ".\STM320518_EVAL_SysTick.bin")

 )

pause

