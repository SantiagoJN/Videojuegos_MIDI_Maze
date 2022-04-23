import os
import shutil
# Iterar sobre los nombres de la carpeta caras
for archivo in os.listdir("colores"):
    # Si el archivo es una imagen
    if archivo.endswith(".jpg"):
        # Crear una carpeta para cada cara
        nombre = archivo.split(".")[0]
        # os.mkdir(nombre)
        # Copiar la imagen a la carpeta
        # os.remove(f'{nombre}/{nombre}.obj')
        # shutil.copyfile(f'yellow/yellow.mtl', f'{nombre}/{nombre}.mtl')
        #os.system("cp " + nombre + " ")
        
        #os.rename(os.path.join("colores", archivo), os.path.join(nombre, archivo))

        cont = 0
        f = open(f'{nombre}/{nombre}.mtl', "w")
        with open('yellow/yellow.mtl', "r") as a_file:
            for line in a_file:
                if cont != 12:
                    f.write(line)
                else:
                    f.write(f'map_Kd  {nombre}.jpg\n')
                cont += 1
        
        f.close()
