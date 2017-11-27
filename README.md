# UNAM - Dispositivos de Almacenamiento y de Entrada y Salida
Hola.

En este repositorio encontrarás el código fuente para las prácticas que involucran el uso del puerto paralelo, el puerto serial y una conexión de red (puede ser vía Ethernet o Wi-Fi). Para en la primer práctica, escribir datos a una memoria RAM y en las siguientes prácticas mover un motor a pasos (generalmente son las 6, 7, 8, 9 y 10).

El código fuente fue escrito en [C++](http://www.cprogramming.com/) y [Java](https://docs.oracle.com/javase/8/docs/api/), además vas a necesitar las librerias:
```
inpout32.dll
inpoutx64.dll
```
Que también puedes encontrar en:
http://www.highrez.co.uk/downloads/inpout32/

**Update:** Ya agregué las librerias al repo para que no batalles.

Nota: Asegúrate de que las librerias estén el la misma carpeta que tus binarios.

Descripción de los archivos
---------------------------
  
  ```
  PParallelLED.ccp
  ```
  * Este código envía 1 byte al registro de salida del puerto paralelo, los valores que recibe son decimales entre 0 y 255, que son transformados a binario y posteriormente se envían al registro de salida del puerto paralelo, puedes ver la salida poniendo unos leds.
  
  ```  
  PParallelDisplay.cpp
  ```
  * Este código hace lo mismo que `PParallelLED.cpp`, la diferencia radica en que está codificado para mostrar las letras del abecedario inglés (no hay *ñ*, pero puedes añadirla tú), los números del 0 al 9 y los caratéres `.` y `-`. Esto con el fin de hacer la escritura de carateres en la memoria RAM más fácilmente. Tomar en cuenta que este programa esta codificado para trabajar con un display de ánodo común (prende con ceros). Tal vez lo arregle para trabajar con cátodo común. Tal vez.
  
  ```  
  PParallelMotor.cpp
  ```
  * El código de este archivo crea un programa que recibe datos de los pines 11, 10 y 12 del puerto paralelo (registro de estados, y sí en ese órden de pines) y envia instrucciones al primer nibble (pines 2, 3, 4 y 5) del registro de entrada del puerto, para mover un motor a pasos conforme a lo que se describe en la sigueinte tabla.

    | Entrada (11,10,12)  | Salida (2, 3, 4, 5) |
    | ------------------  | ------------------- |
    | 111                 | 1080° izquierda     |
    | 000                 | 720° derecha        |
    | 101                 | 180° izquierda      |
    | 110                 | 360° derecha        |
  
  ```
  ClienteSerie.ccp
  ServidorSerie.cpp
  ```
  * Estos archivos se usan en conjunto para lograr una comunicación cliente <-> servidor por el puerto serie, una vez realizada la conexión, se envían instrucciones al puerto paralelo para mover el motor a pasos.
  
  ```
  ClienteWifi.java
  ServidorWifi.java
  MoverMotor.cpp
  ```
  * Nuevamente se utiliza una arquitectura cliente <-> servidor basado en sockets sin cifrado; la implemementación fue hecha con Wi-Fi pero no debería tener ningun problema al usarse en redes alámbricas. Aquí el servidor recibe las instrucciones del cliente y manda a llamar al binario MoverMotor.exe (compilado a partir del archivo `MoverMotor.cpp`), se envian los parámentros especificados y el motor a pasos se mueve conforme a lo especificado.
  
## Notas respecto a la práctica Wi-Fi

  * El código de Java fue probado y compilado usando el JDK versión 7
  * El código en C++ fue probado y compilado usando Code::Blocks
  * El código ha sido probado en cualquiera de las dos primeras máquinas de la primera fila a lado de la ventana, las máquinas de la segunda fila de enmedio no funcionaron para este código.
  * Se utilizó el adaptador Tenda W311MI compatible con XP. Está es la liga del producto:  http://www.tendacn.com/en/product/W311MI.html (Al instalar el driver no es necesario reiniciar el equipo)
  * A veces la conexión Ad-hoc se desconecta y ya no era posible volverse a conectar servidor y el cliente; se debe crear otra red Ad-hoc para que funcione nuevamente.

Bugs, errores, reclamaciones o dudas puedes hacerlas al correo alayanth@gmail.com

Adiós!
