@if exist .\..\TrueSTUDIO\STM320518_EVAL\Debug\STM320518_EVAL.elf ("arm-elf-objcopy.exe" -O binary ".\..\TrueSTUDIO\STM320518_EVAL\Debug\STM320518_EVAL.elf" "STM320518_EVAL_SysTick.bin")

pause

