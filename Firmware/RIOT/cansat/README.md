CanSat
============

Firmware para el microcontrolador supervisor que se encuentra dentro del CanSat. Para poder compilar el firmware primero es necesario
 configurar el ambiente de desarrollo. El siguiente setup fue testeado en Windows 10 64-bit pero debería funcionar también en otras versiones
 de Windows.
  

**1 - Instalar ARM GCC Toolchain**
   - Bajar e instalar el toolchain desde [here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).  
   - Para la carpeta de instalación elegir una ruta de destino **corta** y **sin espacios** como por ejemplo "C:/armgcc":

     ![ARM GCC Path](http://i60.tinypic.com/2l9kvhz.jpg)

   - Seleccionar la opción "Add path to environment variable", deseleccionar todas las demás opciones:
   
   ![Add to Environment Paths](http://i61.tinypic.com/xqfh9t.jpg)

**2 - Instalar MinGW and MSYS**
   - Descargar MINGW desde [here](https://sourceforge.net/projects/mingw/files/latest/download) e iniciar la instalación. Para la carpeta de instalación elegir una ruta de destino **corta** y **sin espacios** como por ejemplo "C:\MinGW":

   ![MinGW Path](http://i61.tinypic.com/1zbs27a.jpg)

   - Seleccionar los siguientes elementos para instalar: **mingw32-base**, **mingw32-gcc-g++**, **msys-base**

   ![MINGW Items](http://i62.tinypic.com/b8oimp.jpg)

   - Instalar también el terminal Mintty que se encuentra dentro de "All Packages": **msys-mintty**

   ![MINTTY](http://i59.tinypic.com/152nsee.jpg)

   - Hacer click en "**Apply Changes**" dentro del menú "**Installation**". 

   - Ir a la ruta de instalación de MinGW y buscar el archivo "mintty.exe" que debiera estar en "**C:\MinGW\msys\1.0\bin**" y crear un acceso directo al mismo.
    
   - Hacer click derecho e ir a "Propiedades" del acceso directo y cambiar el "Destino" a: "**C:\MinGW\msys\1.0\bin\mintty.exe /bin/bash -l**"

   ![MINTTY Target](http://i57.tinypic.com/3097p6d.jpg)

   - Abrir la consola MINTTY con el acceso directo creado y ejecutar el siguiente comando en ella: **mingw-get update**

   ![MINGW Updates](http://i62.tinypic.com/x56qtj.jpg)

  - Ejecutar luego lo siguientes comandos uno por uno "**mingw-get install**" command: **msys-wget**, **msys-zlib**, **msys-unzip**, **msys-mktemp**

   ![MINGW Tools](http://i61.tinypic.com/dnewdw.jpg)

  - Agregar las siguientes rutas a las "Variables de Entorno" del usuario: "**C:\MinGW\bin**", "**C:\MinGW\msys\1.0\bin**"

   ![Environment Variables](http://i62.tinypic.com/34xiyie.jpg)

**4 - Instalar Python 3**

  - Descargar la última versión de **Python 3** desde [aquí](https://www.python.org/downloads/).
  
  - Una vez instalado Python 3 ir al directorio de instalación que suele ser "C:\Users\{usuario}\AppData\Local\Programs\Python\Python37-32".
  
  - Debemos crear un symlink para que el comando "python3" sea reconocido como una llamada a "python.exe". Para ello abrimos una consola y nos dirigimos a "C:\Users\{usuario}\AppData\Local\Programs\Python\Python37-32".
  
  - Ejecutar el siguiente comando para crear el symlink: **mklink "python3.exe" "python.exe"**

**3 - Listo!**

Para más información puede consultarse la fuente original de esta guía en [la documentación de RIOT OS](https://github.com/RIOT-OS/RIOT/wiki/Build-RIOT-on-Windows-OS).
