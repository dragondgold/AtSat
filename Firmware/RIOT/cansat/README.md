CanSat
============

Firmware para el microcontrolador supervisor que se encuentra dentro del CanSat. Para poder compilar el firmware primero es necesario
 configurar el ambiente de desarrollo. Para ellos necesitamos utilizar cualquier distribución de Linux, en nuestro caso se utilizó
 Ubuntu 18.04. Compilar en Windows puede ser posible a través de MinGW o WSL de Windows 10, sin embargo, MinGW trajo algunos problemas y
 WSL no fue testeado.

**1 - Instalar ARM GCC Toolchain**

Para instalar el toolchain simplemente ejecutamos `sudo apt-get install gcc-arm-none-eabi`

**2 - Instalar make**

Ejecutamos `sudo apt-get install make`

**3 - Listo!**

Ya podemos compilar RIOT OS! En este caso se utilizó Visual Studio Code con el plugin `RIOT` como entorno de desarrollo.
