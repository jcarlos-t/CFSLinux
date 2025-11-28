# Red-Black Tree - Algoritmos y Estructuras de Datos

## Descripción

Implementación de un **Árbol Rojo-Negro (Red-Black Tree)** en C++ con sistema de benchmarking integrado. Este proyecto se desarrolló como parte del curso de Algoritmos y Estructuras de Datos, siguiendo el pseudocódigo y especificaciones presentadas por **Thomas H. Cormen** en su libro *"Introduction to Algorithms"*.

## Base Teórica

La implementación sigue fielmente los algoritmos descritos en el capítulo 13 del libro de Cormen et al., incluyendo:

- **Inserción con balanceo automático** mediante rotaciones y recoloreo
- **Eliminación con preservación de propiedades** del árbol rojo-negro
- **Búsqueda de elementos**, predecesor y sucesor
- **Mantenimiento de las 5 propiedades** fundamentales:
  1. Todo nodo es rojo o negro
  2. La raíz es negra
  3. Todas las hojas (NIL) son negras
  4. Ambos hijos de un nodo rojo son negros
  5. Todos los caminos desde un nodo hasta sus hojas descendientes contienen el mismo número de nodos negros

## Estructura del Proyecto

```
RedBlackTree-AED/
│
├── RB_tree.h                    # Implementación principal del árbol rojo-negro
├── main.cpp                     # Interfaz interactiva y sistema de menú
│
├── benchmarks/
│   ├── RB_tree_benchmark.h      # Árbol unificado para mediciones de rendimiento
│   ├── benchmarks_seconds.h     # Benchmark de tiempo de ejecución
│   ├── benchmarks_comparisons.h # Benchmark de número de comparaciones
│   ├── benchmarks_space.h       # Benchmark de uso de memoria y altura
│   └── csv_export.h             # Utilidades para exportar resultados a CSV
│
├── benchmarks_out/              # Carpeta de salida para archivos CSV (creada automáticamente)
│
└── CMakeLists.txt              # Configuración de compilación
```

### Componentes Principales

#### 1. `RB_tree.h`
Implementación template del árbol rojo-negro con las siguientes operaciones:

- `add_leaf(T key)` - Inserción con complejidad O(log n)
- `delete_leaf(T key)` - Eliminación con complejidad O(log n)
- `find(T key)` - Búsqueda con complejidad O(log n)
- `predecesor(T key)` - Encuentra el predecesor de un elemento
- `sucesor(T key)` - Encuentra el sucesor de un elemento
- `print_tree()` - Visualización del árbol con recorrido inorden y preorden

Cada método incluye documentación de su complejidad temporal.

#### 2. `main.cpp`
Interfaz de usuario interactiva con menú que permite:

**Operaciones Básicas:**
- Insertar elementos individualmente
- Buscar elementos en el árbol
- Buscar predecesor y sucesor
- Eliminar elementos
- Visualizar la estructura del árbol

**Benchmarks Automatizados:**
- **Benchmark de Tiempo**: Mide el tiempo de ejecución de INSERT, SEARCH y DELETE
- **Benchmark de Comparaciones**: Cuenta el número de comparaciones realizadas
- **Benchmark de Espacio**: Analiza altura del árbol y uso de memoria

Todos los benchmarks se ejecutan con 4 tamaños de datos: 10,000, 50,000, 100,000 y 200,000 elementos.

#### 3. Sistema de Benchmarking

El sistema realiza **10 repeticiones** por cada prueba y calcula el **promedio** para obtener mediciones más precisas y estables. Esto permite:

- Reducir variabilidad por orden de inserción aleatorio
- Obtener altura promedio del árbol en lugar de un valor fijo
- Proporcionar resultados más representativos del comportamiento real

**Métricas analizadas:**
- Tiempo de ejecución (segundos)
- Número de comparaciones realizadas
- Altura real del árbol vs. altura teórica máxima (2·log₂(n+1))
- Uso de memoria (bytes, KB, MB)

**Exportación de resultados:**

Los resultados de todos los benchmarks se exportan automáticamente a archivos CSV en la carpeta `benchmarks_out/`:
- `benchmark_time_[timestamp].csv` - Resultados de tiempo de ejecución
- `benchmark_comparisons_[timestamp].csv` - Resultados de comparaciones
- `benchmark_space_[timestamp].csv` - Resultados de uso de memoria y altura

Los archivos CSV incluyen todos los datos en formato tabular para facilitar su análisis en herramientas como Excel, Python (pandas), R, o cualquier software de análisis de datos.

## Uso de la Interfaz

Al ejecutar el programa, se presenta un menú interactivo:

```
        ÁRBOL ROJO-NEGRO - MENÚ INTERACTIVO
==================================================
  OPERACIONES BÁSICAS:
    1. Insertar elemento
    2. Buscar elemento
    3. Buscar predecesor
    4. Buscar sucesor
    5. Eliminar elemento
    6. Imprimir árbol

  BENCHMARKS:
    7. Benchmark - TIEMPO
    8. Benchmark - COMPARACIONES
    9. Benchmark - ESPACIO

    0. Salir
==================================================
```

Los benchmarks generan tablas formateadas con los resultados de las pruebas, incluyendo comparativas entre diferentes tamaños de datos.

## Análisis de Complejidad

Todas las operaciones principales mantienen complejidad **O(log n)** gracias al balanceo automático:

| Operación | Complejidad Temporal | Complejidad Espacial |
|-----------|---------------------|----------------------|
| Inserción | O(log n)            | O(1)                |
| Búsqueda  | O(log n)            | O(1)                |
| Eliminación | O(log n)          | O(1)                |
| Predecesor/Sucesor | O(log n)   | O(1)                |

El árbol garantiza que la altura máxima sea a lo sumo **2·log₂(n+1)**, donde n es el número de nodos.

## Mejoras Futuras

1. **Implementación recursiva optimizada**: La implementación actual utiliza punteros al padre (`parent`) como se especifica en Cormen, lo cual facilita el recorrido hacia arriba del árbol pero agrega overhead de memoria. Una alternativa sería aprovechar completamente la recursión eliminando estos punteros, reduciendo el uso de memoria por nodo.

2. **Soporte para iteradores**: Implementar iteradores STL-compatible para facilitar el recorrido del árbol.

3. **Operaciones adicionales**: 
   - Rango de búsqueda (encontrar todos los elementos entre dos valores)
   - Selección del k-ésimo elemento más pequeño
   - Unión e intersección de árboles

4. **Visualización gráfica**: Generar representaciones visuales del árbol en formato gráfico (SVG, DOT).

5. **Persistencia**: Serialización y deserialización para guardar/cargar árboles desde archivos.

6. **Multithreading**: Implementación de operaciones concurrentes con sincronización adecuada.

## Referencias

- **Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C.** (2009). *Introduction to Algorithms* (3rd ed.). MIT Press. Capítulo 13: Red-Black Trees.