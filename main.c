#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------- ESTRUCTURAS ----------

typedef struct Producto {
    char nombre[100];
    float costo;
    struct Producto* siguiente;
    struct Producto* anterior;
} Producto;

typedef struct Usuario {
    char nombre[100];
    char numeroCelular[11]; // +1 por el '\0'
    float totalPagar;
    Producto* carrito;
} Usuario;

// ---------- PROTOTIPOS ----------

Producto* cargarProductosDesdeArchivo(const char* nombreArchivo);
void mostrarMenu();
void mostrarInformacionUsuario(Usuario* usuario);
void mostrarCarrito(Usuario* usuario);
void navegarProductos(Producto* listaProductos, Usuario* usuario);
void agregarAlCarrito(Usuario* usuario, Producto* productoActual);
Producto* copiarProducto(Producto* original);

// ---------- FUNCIONES  ----------
// 
void mostrarMenu() {
    printf("\n--- Menú Principal ---\n");
    printf("1. Ver mi carrito de compras\n");
    printf("2. Ver mi información de usuario\n");
    printf("3. Ver la lista de productos\n");
    printf("4. Salir\n");
    printf("Seleccione una opción: ");
}

void mostrarInformacionUsuario(Usuario* usuario) {
    printf("\n--- Información del Usuario ---\n");
    printf("Nombre: %s\n", usuario->nombre);
    printf("Celular: %s\n", usuario->numeroCelular);
    printf("\n-------------------------------\n");
    printf("Total a pagar: [ $ %.2f ]\n", usuario->totalPagar);
}

void mostrarCarrito(Usuario* usuario) {
    if (usuario->carrito == NULL) {
        printf("\nTu carrito está vacío. ¡Ve a ver productos!\n");
        return;
    }

    printf("\n--- Carrito de Compras ---\n");
    Producto* actual = usuario->carrito;
    while (actual != NULL) {
        printf("Producto: %s - $%.2f\n", actual->nombre, actual->costo);
        actual = actual->siguiente;
    }
    printf("Total a pagar: [ $ %.2f ]\n", usuario->totalPagar);
}

void navegarProductos(Producto* listaProductos, Usuario* usuario) {
    Producto* actual = listaProductos;
    char opcion;

    while (1) {
        printf("\nProducto: %s\nCosto: $%.2f\n", actual->nombre, actual->costo);
        printf("Opciones:\n[A]nterior [S]iguiente [C]omprar [E]salir\n");
        printf("Elija una opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case 'A':
            case 'a':
                if (actual->anterior != NULL) actual = actual->anterior;
                else printf("Ya estás en el primer producto.\n");
                break;
            case 'S':
            case 's':
                if (actual->siguiente != NULL) actual = actual->siguiente;
                else printf("Ya estás en el último producto.\n");
                break;
            case 'C':
            case 'c':
                agregarAlCarrito(usuario, actual);
                printf("¡Producto agregado al carrito!\n");
                break;
            case 'E':
            case 'e':
                return;
            default:
                printf("Opción no válida.\n");
        }
    }
}

void agregarAlCarrito(Usuario* usuario, Producto* productoActual) {
    Producto* nuevo = copiarProducto(productoActual);

    nuevo->siguiente = usuario->carrito;
    nuevo->anterior = NULL;

    if (usuario->carrito != NULL)
        usuario->carrito->anterior = nuevo;

    usuario->carrito = nuevo;
    usuario->totalPagar += nuevo->costo;
}

Producto* copiarProducto(Producto* original) {
    Producto* nuevo = (Producto*)malloc(sizeof(Producto));
    if (nuevo == NULL) {
        printf("Error al asignar memoria\n");
        exit(1);
    }

    strcpy(nuevo->nombre, original->nombre);
    nuevo->costo = original->costo;
    nuevo->siguiente = NULL;
    nuevo->anterior = NULL;

    return nuevo;
}

Producto* cargarProductosDesdeArchivo(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return NULL;
    }

    Producto *inicio = NULL, *anterior = NULL;

    char linea[128];
    while (fgets(linea, sizeof(linea), archivo)) {
        Producto* nuevo = (Producto*)malloc(sizeof(Producto));
        if (!nuevo) {
            printf("Error de memoria.\n");
            exit(1);
        }

        sscanf(linea, "%[^,],%f", nuevo->nombre, &nuevo->costo);
        nuevo->siguiente = NULL;
        nuevo->anterior = anterior;

        if (inicio == NULL)
            inicio = nuevo;
        else
            anterior->siguiente = nuevo;

        anterior = nuevo;
    }

    fclose(archivo);
    return inicio;
}


int main() {
    Usuario usuario = {"Karla Perez", "5512345678", 0.0, NULL};
    Producto* listaProductos = cargarProductosDesdeArchivo("productos.txt");

    if (listaProductos == NULL) {
        printf("No se pudieron cargar los productos.\n");
        return 1;
    }

    int opcion;

    do {
        mostrarMenu();
        scanf("%d", &opcion);
        getchar(); // limpiar buffer

        switch (opcion) {
            case 1:
                mostrarCarrito(&usuario);
                break;
            case 2:
                mostrarInformacionUsuario(&usuario);
                break;
            case 3:
                navegarProductos(listaProductos, &usuario);
                break;
            case 4:
                printf("Saliendo de la tiendita... ¡Gracias por visitarnos!\n");
                break;
            default:
                printf("Opción no válida. Intenta de nuevo :(.\n");
        }

    } while (opcion != 4);

    return 0;
}