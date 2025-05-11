# 🎵 Spotifind

**Spotifind** es una aplicación de consola escrita en C que permite gestionar y buscar canciones desde un archivo CSV. Ofrece búsquedas por género, artista y tempo, facilitando la creación de listas personalizadas con acceso rápido a los datos.

## 📁 Estructura del Proyecto

Asegúrate de contar con los siguientes archivos:

- `spotifind.c` — Archivo principal.
- `hashmap.c/.h`, `list.c/.h`, `extra.h` — Archivos dentro del directorio `tdas/`.
- `song_dataset_.csv` — Archivo con los datos de canciones (debe estar en el mismo directorio que el ejecutable).

## ⚙️ Requisitos

- Sistema con compilador C (como GCC).
- Terminal compatible (Git Bash, Linux Bash, etc.).

## 🔧 Compilación y Ejecución

### 1. Compilar

```bash
gcc spotifind.c tdas/*.c -o spotifind
```

> ⚠️ Si tus archivos no están organizados en carpetas, asegúrate de ajustar las rutas correspondientes.

### 2. Ejecutar

```bash
./spotifind
```

## 🧭 Funcionalidades

1.Cargar Canciones desde archivo CSV
Esta opción permite leer el archivo song_dataset_.csv, extraer los datos de cada canción y almacenarlos en memoria utilizando estructuras eficientes como listas y mapas hash. Es fundamental ejecutar esta opción primero, ya que las demás funcionalidades dependen de que los datos estén cargados correctamente. El archivo debe tener un formato específico, con columnas como id, artists, album_name, track_name, tempo y track_genre.

2.Buscar por Género
El usuario puede ingresar un género musical (por ejemplo, "anime", "rock", "samba", "electro", etc.), y el programa mostrará todas las canciones que pertenezcan a dicho género. La búsqueda es insensible a mayúsculas/minúsculas, lo que facilita la entrada. Internamente se accede al mapa de géneros para recuperar eficientemente la lista de canciones asociadas a ese género.

3.Buscar por Artista
Esta funcionalidad permite al usuario ingresar el nombre de un artista y visualizar todas las canciones asociadas a él/ella. La búsqueda también es insensible a mayúsculas/minúsculas, lo cual ayuda a evitar errores por diferencias de capitalización. Se utiliza un mapa hash que almacena listas de canciones por cada artista, haciendo que la búsqueda sea rápida y precisa incluso si hay muchos datos cargados.

4.Buscar por Tempo
Aquí el usuario puede clasificar las canciones según su velocidad rítmica. Debe seleccionar entre tres categorías:

lento: para canciones con tempo bajo.

moderado: para canciones de tempo medio.

rapido: para canciones con tempo elevado.
La clasificación se basa en rangos definidos de BPM (Beats Per Minute), agrupando automáticamente las canciones en cada categoría para facilitar la exploración musical por ritmo.

5.Finalizar el programa
Esta opción permite cerrar de forma segura la aplicación. Aunque actualmente no libera toda la memoria dinámica usada, es el paso final para salir del menú principal y terminar la ejecución del programa.

## 🚨 Posibles Errores

| Problema | Causa | Solución |
|---------|-------|----------|
| ❌ No se abre el archivo CSV | No existe o está mal ubicado | Asegúrate de que `song_dataset_.csv` esté en el mismo directorio que el ejecutable |
| ⚠️ Fugas de memoria | Uso de `malloc` sin `free` | Se recomienda liberar la memoria manualmente al finalizar el programa |
| ❗ Datos no encontrados | Entrada no coincide con datos cargados | Revisar ortografía (género/artista no distingue mayúsculas, tempo sí) |
| 🔁 Opción inválida | Número incorrecto en el menú | El menú se recarga y pide ingresar una opción válida |
| 🗃️ CSV mal formateado | Columnas desordenadas o faltantes | Solo funciona con el archivo `song_dataset_.csv` original |
| 🧾 Problemas con caracteres | Codificación no UTF-8 | Convertir el archivo CSV a UTF-8 si aparecen caracteres extraños |
| 🔄 Error al volver al menú | Uso incorrecto del número 1 para retroceder | Usar el 1 solo en la función indicada y luego ingresar opciones válidas |

## 📌 Notas

- El archivo CSV contiene **más de 100 géneros** y numerosos artistas.
- El programa **no distingue mayúsculas/minúsculas** en géneros y artistas.
- Los tempos válidos deben ingresarse **exactamente** como `lento`, `moderado` o `rapido`.
