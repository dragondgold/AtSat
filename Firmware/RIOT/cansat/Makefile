# name of your application
APPLICATION = cansat

# If no BOARD is found in the environment, use this default:
BOARD ?= cansat

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/..

# Directory includes
INCLUDES += -I$(CURDIR)/stm32cube/Inc -I$(CURDIR)/stm32cube/Drivers/STM32F3xx_HAL_Driver/Inc -I$(CURDIR)/stm32cube/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I$(CURDIR)/stm32cube/Drivers/CMSIS/Device/ST/STM32F3xx/Include -I$(CURDIR)/stm32cube/Drivers/CMSIS/Include
INCLUDES += -I$(CURDIR)/stm32cube/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I$(CURDIR)/stm32cube/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc

DIRS += config
USEMODULE += config

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 0

include $(RIOTBASE)/Makefile.include
