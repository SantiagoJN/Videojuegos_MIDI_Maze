# Videojuegos_MIDI_Maze
Repositorio del trabajo práctica de la asignatura de videojuegos de la Universidad de Zaragoza (Unizar) curso 2021-2022. MIDI-Maze)
Cancel changes


Añadir un ejecutable con la ruta del fichero fuente en CMakeLists.txt

Utilidades Cmake:
    Si queremos utilizar una biblioteca de github, se puede añadir al fichero External.cmake (hay un ejemplo en el fichero),
    e incluyendo un .h de esa librería en un fichero fuente, cmake la descargará en una carpeta external, que 
    se creará automáticamente.

Instrucciones de compilación con Cmake:
    Crear un directorio build, dentro del repositorio
    Colocar en él un terminal y ejecutar (cmake ..) (Requiere instalar cmake)
    Siempre que se quieran recompilar los fuentes, colocarse en el directorio build y ejecutar (make)
    Los ejecutables serán generados en una carpeta bin, que se creará automaticamente.