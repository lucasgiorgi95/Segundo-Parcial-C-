#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// Constantes
const int MAX_PRODUCTOS = 50;
const int MAX_CLIENTES = 50;
const int MAX_VENTAS = 100;

// ========== VARIABLES GLOBALES ==========

// Estructuras de datos
struct Producto {
    int codigo;
    string nombre;
    float precio;
    int stock;
};

struct Cliente {
    int codigo;
    string nombre;
    string tipo; // "mayorista" o "minorista"
};

// Vectores para almacenamiento
Producto productos[MAX_PRODUCTOS];
Cliente clientes[MAX_CLIENTES];
int contadorProductos = 0;
int contadorClientes = 0;

// Matriz para ventas [codCliente, codProducto, cantidad]
int ventas[MAX_VENTAS][3];
int contadorVentas = 0;

// Matrices auxiliares para reportes
float totalPorProducto[MAX_PRODUCTOS][2]; // [codProducto, total vendido]
float totalPorCliente[MAX_CLIENTES][2];   // [codCliente, total comprado]

// ========== DECLARACIÓN DE FUNCIONES AUXILIARES ==========
int buscarProductoPorCodigo(int codigo);
int buscarClientePorCodigo(int codigo);
string obtenerNombreCliente(int codCliente);
string obtenerNombreProducto(int codProducto);
float obtenerPrecioProducto(int codProducto);
void inicializarMatrices();
void mostrarEncabezadoProductos();
void mostrarProducto(int indice);

// ========== DECLARACIÓN DE FUNCIONES PRINCIPALES ==========
void mostrarMenu();
void registrarProductos();
void registrarClientes();
void realizarVenta();
void mostrarStock();
void mostrarResumen();
void buscarProducto();
void inicializarDatos();

// ========== FUNCIÓN PRINCIPAL ==========
int main() {
    int opcion;
    
    inicializarDatos();
    
    do {
        mostrarMenu();
        cout << "\nIngrese una opcion: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                registrarProductos();
                break;
            case 2:
                registrarClientes();
                break;
            case 3:
                realizarVenta();
                break;
            case 4:
                mostrarStock();
                break;
            case 5:
                buscarProducto();
                break;
            case 6:
                mostrarResumen();
                break;
            case 0:
                cout << "\nSaliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida. Intente nuevamente.\n";
        }
        
        if(opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while(opcion != 0);
    
    return 0;
}

// ========== IMPLEMENTACIÓN DE FUNCIONES AUXILIARES ==========

int buscarProductoPorCodigo(int codigo) {
    for (int i = 0; i < contadorProductos; i++) {
        if (productos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int buscarClientePorCodigo(int codigo) {
    for (int i = 0; i < contadorClientes; i++) {
        if (clientes[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

string obtenerNombreCliente(int codCliente) {
    int indice = buscarClientePorCodigo(codCliente);
    return (indice != -1) ? clientes[indice].nombre : "Cliente no encontrado";
}

string obtenerNombreProducto(int codProducto) {
    int indice = buscarProductoPorCodigo(codProducto);
    return (indice != -1) ? productos[indice].nombre : "Producto no encontrado";
}

float obtenerPrecioProducto(int codProducto) {
    int indice = buscarProductoPorCodigo(codProducto);
    return (indice != -1) ? productos[indice].precio : 0.0f;
}

void inicializarMatrices() {
    for (int i = 0; i < MAX_PRODUCTOS; i++) {
        totalPorProducto[i][0] = totalPorProducto[i][1] = 0;
    }
    for (int i = 0; i < MAX_CLIENTES; i++) {
        totalPorCliente[i][0] = totalPorCliente[i][1] = 0;
    }
}

void mostrarEncabezadoProductos() {
    cout << left << setw(10) << "Codigo"
         << setw(25) << "Nombre"
         << setw(12) << "Precio"
         << setw(10) << "Stock" << endl;
    cout << string(57, '-') << endl;
}

void mostrarProducto(int indice) {
    cout << left << setw(10) << productos[indice].codigo
         << setw(25) << productos[indice].nombre
         << "$" << setw(11) << fixed << setprecision(2) << productos[indice].precio
         << setw(10) << productos[indice].stock << endl;
}

// Función para inicializar datos
void inicializarDatos() {
    inicializarMatrices();
}

// Función para mostrar el menú principal
void mostrarMenu() {
    system("clear"); // Para Linux/Mac, usar "cls" en Windows
    cout << "========================================\n";
    cout << "   SISTEMA DE GESTION - DISTRIBUIDORA\n";
    cout << "========================================\n";
    cout << "1. Registrar productos\n";
    cout << "2. Registrar clientes\n";
    cout << "3. Mostrar stock de productos\n";
    cout << "4. Buscar producto por codigo\n";
    cout << "5. Realizar venta\n";
    cout << "6. Mostrar resumen de ventas\n";
    cout << "0. Salir\n";
    cout << "========================================\n";
}

// Función para registrar nuevos productos
// Solicita los datos al usuario y los guarda en el arreglo de productos
void registrarProductos() {
    if(contadorProductos >= MAX_PRODUCTOS) {
        cout << endl << "No se pueden registrar mas productos. Limite alcanzado." << endl;
        return;
    }
    
    Producto nuevoProducto;
    cout << endl << "--- REGISTRAR PRODUCTO ---" << endl;
    
    // Solicitar código de producto
    cout << "Ingrese codigo de producto: ";
    cin >> nuevoProducto.codigo;
    
    // Verificar si el código ya existe
    if (buscarProductoPorCodigo(nuevoProducto.codigo) != -1) {
        cout << "Error: El codigo de producto ya existe." << endl;
        return;
    }
    
    cin.ignore(); // Limpiar buffer
    
    // Solicitar nombre del producto
    cout << "Ingrese nombre del producto: ";
    getline(cin, nuevoProducto.nombre);
    
    // Solicitar precio unitario
    cout << "Ingrese precio unitario: $";
    cin >> nuevoProducto.precio;
    
    // Validar precio positivo
    while(nuevoProducto.precio <= 0) {
        cout << "Error: El precio debe ser mayor a 0. Intente nuevamente: $";
        cin >> nuevoProducto.precio;
    }
    
    // Solicitar stock inicial
    cout << "Ingrese stock disponible: ";
    cin >> nuevoProducto.stock;
    
    // Validar stock no negativo
    while(nuevoProducto.stock < 0) {
        cout << "Error: El stock no puede ser negativo. Intente nuevamente: ";
        cin >> nuevoProducto.stock;
    }
    
    // Agregar el nuevo producto al arreglo
    productos[contadorProductos] = nuevoProducto;
    contadorProductos++;
    
    cout << endl << "¡Producto registrado exitosamente!" << endl;
}

// Función para registrar nuevos clientes
// Almacena la información del cliente en el arreglo de clientes
void registrarClientes() {
    if(contadorClientes >= MAX_CLIENTES) {
        cout << endl << "No se pueden registrar más clientes. Límite alcanzado." << endl;
        return;
    }
    
    Cliente nuevoCliente;
    cout << endl << "--- REGISTRAR CLIENTE ---" << endl;
    
    // Generar código de cliente automáticamente (empezando desde 1000)
    nuevoCliente.codigo = 1000 + contadorClientes;
    cout << "Codigo de cliente asignado: " << nuevoCliente.codigo << endl;
    
    cin.ignore(); // Limpiar buffer
    
    // Solicitar nombre del cliente
    cout << "Ingrese nombre del cliente: ";
    getline(cin, nuevoCliente.nombre);
    
    // Solicitar tipo de cliente
    int tipo;
    cout << "Tipo de cliente:\n";
    cout << "1. Mayorista\n";
    cout << "2. Minorista\n";
    cout << "Seleccione opcion: ";
    cin >> tipo;
    
    if(tipo == 1) {
        nuevoCliente.tipo = "Mayorista";
    } else if(tipo == 2) {
        nuevoCliente.tipo = "Minorista";
    } else {
        cout << "Opcion invalida. Se registrara como Minorista.\n";
        nuevoCliente.tipo = "Minorista";
    }
    
    // Agregar el nuevo cliente al arreglo
    clientes[contadorClientes] = nuevoCliente;
    contadorClientes++;
    
    cout << endl << "¡Cliente registrado exitosamente!" << endl;
}

// Función para realizar una venta
// Verifica stock disponible y registra la operación
void realizarVenta() {
    if(contadorProductos == 0) {
        cout << endl << "No hay productos registrados." << endl;
        return;
    }
    
    if(contadorClientes == 0) {
        cout << endl << "No hay clientes registrados." << endl;
        return;
    }
    
    cout << endl << "--- REALIZAR VENTA ---" << endl << endl;
    
    int opcionCliente, codProducto, cantidad;
    
    // Mostrar lista de clientes disponibles
    cout << "=== LISTA DE CLIENTES ===" << endl;
    for(int i = 0; i < contadorClientes; i++) {
        cout << i + 1 << ". " << clientes[i].nombre 
             << " (Codigo: " << clientes[i].codigo 
             << ", Tipo: " << clientes[i].tipo << ")" << endl;
    }
    
    // Solicitar selección de cliente
    cout << endl << "Seleccione el número del cliente: ";
    cin >> opcionCliente;
    
    // Validar selección
    if(opcionCliente < 1 || opcionCliente > contadorClientes) {
        cout << "Opción de cliente inválida." << endl;
        return;
    }
    
    int indiceCliente = opcionCliente - 1;
    cout << endl << "Cliente seleccionado: " << clientes[indiceCliente].nombre 
         << " (" << clientes[indiceCliente].tipo << ")" << endl;
    
    cout << "Cliente: " << clientes[indiceCliente].nombre 
         << " (" << clientes[indiceCliente].tipo << ")" << endl << endl;
    
    // Solicitar código de producto
    cout << "Ingrese codigo de producto: ";
    cin >> codProducto;
    
    // Buscar producto
    int indiceProducto = buscarProductoPorCodigo(codProducto);
    
    if(indiceProducto == -1) {
        cout << "Error: Producto no encontrado." << endl;
        return;
    }
    
    cout << "Producto: " << productos[indiceProducto].nombre << endl;
    cout << "Precio unitario: $" << fixed << setprecision(2) 
         << productos[indiceProducto].precio << endl;
    cout << "Stock disponible: " << productos[indiceProducto].stock << endl << endl;
    
    // Solicitar cantidad
    cout << "Ingrese cantidad a vender: ";
    cin >> cantidad;
    
    // Verificar stock disponible
    if(cantidad > productos[indiceProducto].stock) {
        cout << "Error: Stock insuficiente." << endl;
        return;
    }
    
    if(cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
        return;
    }
    
    // Registrar venta
    ventas[contadorVentas][0] = clientes[indiceCliente].codigo;
    ventas[contadorVentas][1] = codProducto;
    ventas[contadorVentas][2] = cantidad;
    contadorVentas++;
    
    // Descontar stock
    productos[indiceProducto].stock -= cantidad;
    
    // Calcular total de la venta
    float total = cantidad * productos[indiceProducto].precio;
    
    cout << endl << "--- RESUMEN DE VENTA ---" << endl;
    cout << "Total: $" << fixed << setprecision(2) << total << "\n";
    cout << "Stock restante: " << productos[indiceProducto].stock << "\n";
    cout << "\n¡Venta registrada exitosamente!\n";
}

// Función para mostrar el stock de todos los productos
// Lista los productos en formato tabular
void mostrarStock() {
    if(contadorProductos == 0) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    cout << endl << "--- STOCK DE PRODUCTOS ---" << endl << endl;
    mostrarEncabezadoProductos();
    
    for(int i = 0; i < contadorProductos; i++) {
        mostrarProducto(i);
    }
}

// Función para buscar un producto por código
void buscarProducto() {
    if(contadorProductos == 0) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    cout << "\n--- BUSCAR PRODUCTO ---\n";
    int codigo;
    cout << "Ingrese codigo de producto: ";
    cin >> codigo;
    
    int indice = buscarProductoPorCodigo(codigo);
    
    if(indice == -1) {
        cout << endl << "Producto no encontrado." << endl;
        return;
    }
    
    cout << "\n--- INFORMACION DEL PRODUCTO ---\n";
    cout << "Codigo: " << productos[indice].codigo << "\n";
    cout << "Nombre: " << productos[indice].nombre << "\n";
    cout << "Precio: $" << fixed << setprecision(2) << productos[indice].precio << "\n";
    cout << "Stock: " << productos[indice].stock << "\n";
}

// Función para generar resumen de ventas
// Calcula totales por producto y por cliente
void mostrarResumen() {
    if(contadorVentas == 0) {
        cout << endl << "No hay ventas registradas." << endl;
        return;
    }
    
    // Inicializar matrices auxiliares
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        totalPorProducto[i][0] = 0;
        totalPorProducto[i][1] = 0;
    }
    for(int i = 0; i < MAX_CLIENTES; i++) {
        totalPorCliente[i][0] = 0;
        totalPorCliente[i][1] = 0;
    }
    
    // Calcular totales
    for(int i = 0; i < contadorVentas; i++) {
        int codCliente = ventas[i][0];
        int codProducto = ventas[i][1];
        int cantidad = ventas[i][2];
        
        // Obtener precio del producto usando la función auxiliar
        float precio = obtenerPrecioProducto(codProducto);
        float total = cantidad * precio;
        
        // Acumular por producto
        bool productoEncontrado = false;
        for(int j = 0; j < MAX_PRODUCTOS; j++) {
            if(static_cast<int>(totalPorProducto[j][0]) == codProducto) {
                totalPorProducto[j][1] += total;
                productoEncontrado = true;
                break;
            }
            if(static_cast<int>(totalPorProducto[j][0]) == 0 && !productoEncontrado) {
                totalPorProducto[j][0] = static_cast<float>(codProducto);
                totalPorProducto[j][1] = total;
                break;
            }
        }
        
        // Acumular por cliente
        bool clienteEncontrado = false;
        for(int j = 0; j < MAX_CLIENTES; j++) {
            if(static_cast<int>(totalPorCliente[j][0]) == codCliente) {
                totalPorCliente[j][1] += total;
                clienteEncontrado = true;
                break;
            }
            if(static_cast<int>(totalPorCliente[j][0]) == 0 && !clienteEncontrado) {
                totalPorCliente[j][0] = static_cast<float>(codCliente);
                totalPorCliente[j][1] = total;
                break;
            }
        }
    }
    
    // Mostrar resumen por producto
    cout << endl << "--- RESUMEN DE VENTAS POR PRODUCTO ---" << endl << endl;
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(15) << "Total Vendido" << endl;
    cout << string(50, '-') << endl;
    
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        if(totalPorProducto[i][0] != 0) {
            int cod = static_cast<int>(totalPorProducto[i][0]);
            string nombre = obtenerNombreProducto(cod);
            
            cout << left << setw(10) << cod
                 << setw(25) << nombre
                 << "$" << fixed << setprecision(2) 
                 << totalPorProducto[i][1] << endl;
        }
    }
    
    // Mostrar resumen por cliente
    cout << endl << "--- RESUMEN DE VENTAS POR CLIENTE ---" << endl << endl;
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(15) << "Total Comprado" << endl;
    cout << string(50, '-') << endl;
    
    for(int i = 0; i < MAX_CLIENTES; i++) {
        if(totalPorCliente[i][0] != 0) {
            int cod = static_cast<int>(totalPorCliente[i][0]);
            string nombre = obtenerNombreCliente(cod);
            
            cout << left << setw(10) << cod
                 << setw(25) << nombre
                 << "$" << fixed << setprecision(2) 
                 << totalPorCliente[i][1] << endl;
        }
    }
    
    // Calcular total general
    float totalGeneral = 0;
    for(int i = 0; i < MAX_PRODUCTOS; i++) {
        totalGeneral += totalPorProducto[i][1];
    }
    
    cout << endl << string(50, '-') << endl;
    cout << "TOTAL GENERAL DE VENTAS: $" << fixed << setprecision(2) 
         << totalGeneral << endl;
}