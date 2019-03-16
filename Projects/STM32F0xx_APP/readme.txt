/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
/**
  @page binary_template Binary Template Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/binary_template/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the binary_template directory.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
   @endverbatim

@par Description

This directory contains a set of sources files that build the application to be
loaded into Flash memory using In-Application Programming (IAP, through USART).

To build such application, some special configuration has to be performed:
1. Set the application load address at 0x08003000, using your toolchain linker file
2. To be able to serve the application interrupts, you need to relocate the vector 
   table (which contains the interrupt handlers). However, unlike CortexM3 and CortexM4, 
   the CortexM0 processor do not support vector table relocation (it is fixed at 
   address 0x00000000).
   A solution will be to relocate by software the vector table to the internal SRAM:  
    - Copy the vector table from the Flash (mapped at the base of the application load
      address 0x08003000) to the base address of the SRAM at 0x20000000.
    - Remap SRAM at address 0x00000000, using SYSCFG_MemoryRemapConfig() function
    - Then once an interrupt occurs, the CortexM0 processor will fetch the interrupt 
      handler start address from the relocated vector table in SRAM, then it will 
      jump to execute the interrupt handler located in the Flash.
   This operation should be done at the initialization phase of the application.    
   
The SysTick example provided within the STM32F0xx Standard Peripherals library 
examples is used as illustration.
This example configures the SysTick to generate a time base equal to 1 ms.
The system clock is set to the maximum system frequency, the SysTick is clocked by 
the AHB clock (HCLK). A "Delay" function is implemented based on the SysTick 
end-of-count event.
Four LEDs are toggled with a timing defined by the Delay function.


@par Directory contents 

 - "STM32F0xx_IAP\binary_template\EWARM"
 - "STM32F0xx_IAP\binary_template\MDK-ARM"
 - "STM32F0xx_IAP\binary_template\TASKING"
 - "STM32F0xx_IAP\binary_template\TrueSTUDIO" 
     - These folders contains a pre-configured project that produces a binary
       image of SysTick example to be loaded with IAP.

  - "STM32F0xx_IAP\binary_template\inc": contains the binary_template firmware header files 
     - IAP/binary_template/inc/stm32f0xx_conf.h    Library Configuration file
     - IAP/binary_template/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c
     - IAP/binary_template/inc/main.h              Header for main.c

 - "STM32F0xx_IAP\binary_template\src": contains the binary_template firmware source files 
     - IAP/binary_template/src/main.c              Main program
     - IAP/binary_template/src/stm32f0xx_it.c      Interrupt handlers
     - IAP/binary_template/src/system_stm32f0xx.c  STM32F0xx system source file

     
@par Hardware and Software environment 

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.
    
  - STM320518-EVAL Set-up
    - Use LED1, LED2, LED3 and LED4 connected respectively to PC.10, PC.11, PC.12
      and PD.02 pins


@par How to use it ?  

In order to load the SysTick example with the IAP, you must do the following:

 - EWARM
    - Open the Project.eww workspace
    - Rebuild all files: Project->Rebuild all
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under 
      "STM320518-EVAL\Exe" folder.  
    - Finally load this image with the IAP application

 - MDK-ARM
    - Open the Project.uvproj project
    - Rebuild all files: Project->Rebuild all target files
    - Go to "\binary_template\Binary" directory and run "axftobin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TASKING_hextobin.bat".
    - A binary file "STM320518-EVAL.bin" will be generated under this
      directory location. 
    - Finally load this image with IAP application
    
 - TrueSTUDIO
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory, select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Go to "\binary_template\Binary" directory and run "TrueSTUDIO_elf2bin.bat"
    - A binary file "STM320518-EVAL_SysTick.bin" will be generated under this
      directory location.
    - Finally load this image with IAP application


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
