# MIDI Maze
Videojuegos, grupo 1, Curso 2021-2022

![Portada Midi Maze](https://upload.wikimedia.org/wikipedia/en/5/56/MIDI_Maze.jpg)

 El propósito de este README es el de explicar de forma rápida cómo se puede
jugar al MIDI Maze desarrollado, sin necesidad de tener que leer toda la
memoria, aunque si se necesitan más detalles o alguna característica no ha 
quedado clara, recomendamos leer el apartado correspondiente de la memoria.

## Cómo jugar
### Iniciar una partida
 Lo primero que hay que hacer es ejecutar el MidiMaze.exe que se encuentra en
la carpeta "Videojuego". Se abrirá la pantalla de inicio, y se tendrá que 
configurar la partida que se quiere jugar. Para ello se configuran dos cosas:
el mapa en el que jugar, y las características de la partida.

 Para acceder a la configuración, presionar el texto "MAZE" de la parte superior
o la flecha azul, y aparecerá un desplegable.

 Para acceder a la configuración extra, presionar el botón a la izquierda del
texto "MAZE".

 El mapa se carga en la opción "load". Al darle a esta opción, se muestran los
mapas que se encuentran en la carpeta "maps" (se pueden modificar con un editor
de texto; más información en la memoria). Se selecciona un mapa dándole click.

 Si se selecciona la opción de "Play", se pueden elegir los diferentes parámetros
de la partida (más información en la memoria). Una vez elegidos los parámetros
deseados, pulsar "Yeah!" para que empiece la partida!

### Controles
Dentro de la propia partida hay algunos controles:
 - La cámara se mueve con el ratón
 - El personaje se mueve con las teclas WASD (adelante, izquierda, atrás, derecha
	respectivamente)
 - Se dispara con el click derecho
 - Se sube y baja el volumen con la O y la L respectivamente
 - Al darle al botón de escape se pausa la partida, pudiéndola continuar o salir

### En la partida
Las reglas son sencillas: ¡El primer jugador que llegue a 10 asesinatos gana la partida!

## Versión original - Emulador
 Por si alguien tiene curiosidad de ver cómo funciona o las diferencias entre
el juego desarrollado y el original, hemos compartido también los ficheros
necesarios para utilizar un emulador y correr el juego original, y en este
apartado explicamos qué pasos hay que tomar para ejecutarlo.

 Para ello hay que tener en cuenta dos partes: El propio emulador, llamado 
"Hatari", y el juego del MIDI Maze.

 Si solo queréis ver un poco el juego original, hay vídeos en youtube que 
lo muestran, como por ejemplo https://www.youtube.com/watch?v=8hSoy1S43dw

### Configurar el emulador
 Para configurar el emulador hay que seguir unos pocos pasos:
 -Abrir el emulador, el fichero "hatari.exe"
 -Una vez dentro, pulsar F12 para acceder a la configuración
 -Pulsar ROM > TOS Image > Browse, y seleccionar el "etos256uk.img", que se
	encuentra en la misma carpeta que "hatari.exe". Volver al menú de config.
 -Pulsar Floppy Disks > Drive A > Browse (comprobar que está activado, y que
	la opción de "Double sided" está desactivada). Navegar con el explorador
	de archivos y seleccionar el "Midi Maze (1987)(Hybrid Arts)(PD)
	[republished by Karstadt].st", que se encuentra en la carpeta ROMS. Volver
	al menú de config.
 -Guardar los cambios con Save Config > OK
 -Reiniciar la máquina: Pulsar la opción de "Reset machine", y darle a OK.
 -Una vez se reinicie, abrir el Disk A, abrir la carpeta y ejecutar el programa
	"MidiMaze.PRG".
 - Configurar los controles: En las opciones seleccionar "Joystick" > "Use Keyboard"

### Iniciar una partida
 Lo primero que saldrá al abrir el juego es una ventana informándonos que somos
una máquina slave. Hay que pulsar alt + s, y darle a OK cuando nos informe de que
somos la "SOLO Machine". Una vez dentro del juego, se crea una partida igual que
con la versión implementada: Darle al texto "MAZE", "Play", configurar la partida
y empezarla.

### Controles
 Ya que el juego está diseñado para correr en la Atari ST, los controles están
orientados a esta máquina, que utilizaba un joystick, no un ratón. Los controles
son:
 -Flechas derecha e izquierda para mover la cámara
 -Flechas arriba y abajo para andar hacia adelante y hacia detrás
 -Tecla Ctrl izquierdo para disparar
 -Tecla espacio para mostrar el mapa

### En la partida
 Las reglas son las mismas que las del juego implementado.