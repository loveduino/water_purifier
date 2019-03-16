/**
  @page STM32F0xx_IAP STM32F0xx in-application programming using the USART Readme file
  
  @verbatim
  ******************** (C) COPYRIGHT 2012 STMicroelectronics *******************
  * @file    STM32F0xx_IAP/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   Description of the "STM32F0xx in-application programming (IAP) using the USART".
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

//IAP升级
This directory contains a set of sources files and pre-configured projects that 
build the In-Application Programming (IAP, through USART) program.

//自动波特率
This example provides a description of how to use the Auto BaudRate capability
of the USART via the HyperTerminal.

First, the USART1(STM32F051R8T6 devices) or USART2(STM32F072VBT6 devices) is configured
to detect BaudRate automatically with StartBit Measurement Method (Any character 
starting with a bit at 1 should be send to USART to detect Baud Rate).

After configuration, the USART waits until a data is received via HyperTerminal
to launch auto-detection of Baud Rate phase. The end of this phase is monitored 
by ABRF flag.

The result of auto-detection is indicated by EVAL Board LEDs. If an error occurred 
in Auto BaudRate phase ABRE flag is set and LED3 is turned on. If this phase is 
completed successfully, LED2 is turned on and the received data is transmitted back 
to the HyperTerminal.
You can repeat this procedure by changing the BaudRate on HyperTerminal side.
 
The USART is configured as follow:
    - BaudRate = 115200 baud (any value can be chosen)  
    - Word Length = 8 Bits 
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled


@par Directory contents 

 - "STM32F0xx_IAP\binary_template": contains the binary template firmware source files
 - "STM32F0xx_IAP\EWARM": contains preconfigured project for EWARM toolchain
 - "STM32F0xx_IAP\MDK-ARM": contains preconfigured project for MDK-ARM toolchain
 - "STM32F0xx_IAP\TASKING": contains preconfigured project for TASKING toolchain
 - "STM32F0xx_IAP\TrueSTUDIO": contains preconfigured project for TrueSTUDIO toolchain
  
 - "STM32F0xx_IAP\inc": contains the IAP firmware header files 
    - STM32F0xx_IAP/inc/common.h            This file provides all the headers of the common functions.
    - STM32F0xx_IAP/inc/flash_if.h          This file provides all the firmware 
                                            function headers of the flash_if.c file.
    - STM32F0xx_IAP/inc/menu.h              This file provides all the firmware
                                            function headers of the menu.c file.
    - STM32F0xx_IAP/inc/ymodem.h            This file provides all the firmware
                                            function headers of the ymodem.c file.
    - STM32F0xx_IAP/inc/stm32f0xx_conf.h    Library Configuration file
    - STM32F0xx_IAP/inc/stm32f0xx_it.h      Header for stm32f0xx_it.c

 - "STM32F0xx_IAP\src": contains the IAP firmware source files
    - STM32F0xx_IAP/src/main.c              Main program
    - STM32F0xx_IAP/src/stm32f0xx_it.c      Interrupt handlers
    - STM32F0xx_IAP/src/flash_if.c          This file provides all the memory related
                                            operation functions(write, erase and
                                            write protection disable)
    - STM32F0xx_IAP/src/menu.c              This file contains the menu to select
                                            downloading a new binary file, uploading
                                            internal Flash memory, executing the binary
                                            and disabling the write protection of
                                            write protected sectors.
    - STM32F0xx_IAP/src/common.c            This file provides functions related to
                                            read/write from/to USART peripheral.
    - STM32F0xx_IAP/src/ymodem.c            This file provides all the firmware functions
                                            related to the ymodem protocol.
    - STM32F0xx_IAP/src/system_stm32f0xx.c  STM32F0xx system source file. 


@par Hardware and Software environment

  - This example runs on STM32F0xx Devices.
  
  - This example has been tested with STMicroelectronics STM320518-EVAL (STM32F0xx) 
    evaluation board and can be easily tailored to any other supported device 
    and development board.

  - STM320518-EVAL Set-up
      - Connect a null-modem female/female RS232 cable between the DB9 connector 
        CN7 and PC serial port.
    - Use the Key push-button.
   
  - Hyperterminal configuration:
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 


@par How to use it ? 

In order to make the program work, you must do the following:

  1. Generate a binary image using the preconfigured project provided in the directory
      "Project\STM32F0xx_IAP\binary_template" . 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP application, keep the Key push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

  In order to load the IAP code, you have do the following:
 - EWARM
    - Open the STM320518-EVAL_Demo.eww workspace
    - Rebuild all files: Project->Rebuild all
    - Load project image: Project->Debug
    - Run program: Debug->Go(F5)

 - MDK-ARM 
    - Open the Project.Uv2 project
    - Rebuild all files: Project->Rebuild all target files
    - Load project image: Debug->Start/Stop Debug Session
    - Run program: Debug->Run (F5)

 - TASKING
    - Open TASKING toolchain.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TASKING workspace directory and select the project: STM320518-EVAL
     - Rebuild all project files: Select the project in the "Project explorer" 
       window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 - TrueSTUDIO   
    - Open the TrueSTUDIO toolchain.
    - Click on File->Switch Workspace->Other and browse to TrueSTUDIO workspace 
      directory.
    - Click on File->Import, select General->'Existing Projects into Workspace' 
      and then click "Next". 
    - Browse to the TrueSTUDIO workspace directory and select the project: STM320518-EVAL
    - Rebuild all project files: Select the project in the "Project explorer" 
      window then click on Project->build project menu.
    - Run program: Select the project in the "Project explorer" window then click 
      Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
