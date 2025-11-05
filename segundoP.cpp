#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Constantes
template<typename T, size_t N> constexpr size_t array_size(T (&)[N]) { return N; }
const int MAX_PRODUCTOS = 50, MAX_CLIENTES = 50, MAX_VENTAS = 100;

// Estructuras
template<typename T> struct Registro { int codigo; string nombre; T extra; int stock = 0; };
using Producto = Registro<float>;
using Cliente = Registro<string>;

// Variables globales
Producto productos[MAX_PRODUCTOS];
Cliente clientes[MAX_CLIENTES];
int contadores[3] = {0}; // [0]=productos, [1]=clientes, [2]=ventas
int ventas[MAX_VENTAS][3];

// Prototipos
void limpiarPantalla();
template<typename T> T leerValor(const string& mensaje, T min = numeric_limits<T>::min(), T max = numeric_limits<T>::max());
int buscarPorCodigo(const auto& arr, int size, int codigo);
void mostrarEncabezado(const string& titulo);
void mostrarMenu();
void registrarProductos();
void registrarClientes();
void realizarVenta();
void mostrarStock();
void buscarProducto();
void mostrarResumenVentas();

// ========== FUNCIÓN PRINCIPAL ==========
int main() {
    const string opciones[] = {"Salir", "Registrar productos", "Registrar clientes", 
                             "Mostrar stock", "Buscar producto", "Realizar venta", 
                             "Mostrar resumen de ventas"};
    void (*acciones[])() = {nullptr, registrarProductos, registrarClientes, 
                           mostrarStock, buscarProducto, realizarVenta, mostrarResumenVentas};
    
    int opcion;
    do {
        mostrarMenu();
        opcion = leerValor<int>("\nIngrese una opcion: ", 0, array_size(opciones)-1);
        if (opcion > 0) acciones[opcion]();
        if (opcion != 0) {
            cout << "\nPresione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
    } while (opcion != 0);
    
    cout << "\nSaliendo del sistema...\n";
    return 0;
}

// Funciones auxiliares
void limpiarPantalla() { system("cls || clear"); }

template<typename T>
T leerValor(const string& mensaje, T min, T max) {
    T valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor >= min && (max == numeric_limits<T>::max() || valor <= max)) 
            return valor;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Valor inválido. " << (min != numeric_limits<T>::min() ? 
            "Debe ser mayor o igual a " + to_string(min) + 
            (max != numeric_limits<T>::max() ? " y menor o igual a " + to_string(max) : "") : "") << ".\n";
    }
}

int buscarPorCodigo(const auto& arr, int size, int codigo) {
    for (int i = 0; i < size; i++)
        if (arr[i].codigo == codigo) return i;
    return -1;
}

void mostrarEncabezado(const string& titulo) {
    limpiarPantalla();
    cout << "--- " << titulo << " ---\n\n";
}

// ========== IMPLEMENTACIÓN DE FUNCIONES PRINCIPALES ==========

void limpiarPantalla() { system("cls || clear"); }
void pausa() { cout << "\nPresione Enter para continuar..."; cin.ignore(); cin.get(); }

void mostrarMenu() {
    limpiarPantalla();
    const char* menu[] = {
        "========================================",
        "   SISTEMA DE GESTION - DISTRIBUIDORA",
        "========================================",
        "1. Registrar productos",
        "2. Registrar clientes",
        "3. Mostrar stock de productos",
        "4. Buscar producto por código",
        "5. Realizar venta",
        "6. Mostrar resumen de ventas",
        "0. Salir",
        "========================================"
    };
    for (const auto& linea : menu) cout << linea << '\n';
}

void registrarProductos() {
    if (contadores[0] >= MAX_PRODUCTOS) {
        cout << "\nNo se pueden registrar más productos. Límite alcanzado.\n";
        return;
    }
    
    mostrarEncabezado("REGISTRAR PRODUCTO");
    
    Producto p;
    p.codigo = leerValor<int>("Código de producto: ");
    
    if (buscarPorCodigo(productos, contadores[0], p.codigo) != -1) {
        cout << "Error: El código ya existe.\n";
        return;
    }
    
    cout << "Nombre: ";
    cin.ignore();
    getline(cin, p.nombre);
    p.extra = leerValor<float>("Precio unitario: $", 0.01f);
    p.stock = leerValor<int>("Stock inicial: ", 0);
    
    productos[contadores[0]++] = p;
    cout << "\n¡Producto registrado exitosamente!\n";
}

void registrarClientes() {
    if (contadores[1] >= MAX_CLIENTES) {
        cout << "\nNo se pueden registrar más clientes. Límite alcanzado.\n";
        return;
    }
    
    mostrarEncabezado("REGISTRAR CLIENTE");
    
    Cliente c;
    c.codigo = 1000 + contadores[1];
    cout << "Código asignado: " << c.codigo << "\n";
    
    cout << "Nombre: ";
    cin.ignore();
    getline(cin, c.nombre);
    
    int tipo = leerValor<int>("Tipo de cliente (1:Mayorista, 2:Minorista): ", 1, 2);
    c.extra = (tipo == 1) ? "Mayorista" : "Minorista";
    
    clientes[contadores[1]++] = c;
    cout << "\n¡Cliente registrado exitosamente!\n";
}

void realizarVenta() {
    if (!contadores[0] || !contadores[1]) {
        cout << "\n" << (!contadores[0] ? "No hay productos" : "No hay clientes") 
             << " registrados.\n";
        return;
    }
    
    mostrarEncabezado("REALIZAR VENTA");
    
    // Selección de cliente
    cout << "=== LISTA DE CLIENTES ===\n";
    for (int i = 0; i < contadores[1]; i++)
        cout << i + 1 << ". " << clientes[i].nombre << " (" << clientes[i].extra << ")\n";
    
    int opcionCliente = leerValor<int>("\nSeleccione cliente: ", 1, contadores[1]) - 1;
    cout << "\nCliente: " << clientes[opcionCliente].nombre 
             << " (" << clientes[opcionCliente].extra << ")\n\n";
    
    // Selección de producto
    int codigo = leerValor<int>("Código de producto: ");
    int indiceProducto = buscarPorCodigo(productos, contadores[0], codigo);
    
    if (indiceProducto == -1) {
        cout << "Producto no encontrado.\n";
        return;
    }
    
    Producto& p = productos[indiceProducto];
    cout << "Producto: " << p.nombre << " - Stock: " << p.stock 
         << " - Precio: $" << fixed << setprecision(2) << p.extra << "\n";
    
    // Validar cantidad
    int cantidad = leerValor<int>("Cantidad: ", 1, p.stock);
    
    // Procesar venta
    p.stock -= cantidad;
    float total = cantidad * p.extra;
    
    // Registrar venta
    ventas[contadores[2]][0] = clientes[opcionCliente].codigo;
    ventas[contadores[2]][1] = p.codigo;
    ventas[contadores[2]][2] = cantidad;
    contadores[2]++;
    
    cout << "\n--- RESUMEN DE VENTA ---\n"
         << "Total: $" << fixed << setprecision(2) << total << "\n"
         << "Stock restante: " << p.stock << "\n"
         << "\n¡Venta registrada exitosamente!\n";
}

void mostrarProductos() {
    if (!contadores[0]) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    mostrarEncabezado("STOCK DE PRODUCTOS");
    
    cout << left << setw(10) << "Código" << setw(25) << "Nombre"
         << setw(12) << "Precio" << setw(10) << "Stock" << "\n"
         << string(57, '-') << '\n';
    
    for (int i = 0; i < contadores[0]; i++) {
        cout << left << setw(10) << productos[i].codigo
             << setw(25) << productos[i].nombre
             << "$" << setw(11) << fixed << setprecision(2) << productos[i].extra
             << setw(10) << productos[i].stock << '\n';
    }
}

void buscarProducto() {
    if (!contadores[0]) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    mostrarEncabezado("BUSCAR PRODUCTO");
    
    int codigo = leerValor<int>("Ingrese código de producto: ");
    int indice = buscarPorCodigo(productos, contadores[0], codigo);
    
    if (indice == -1) {
        cout << "\nProducto no encontrado.\n";
        return;
    }
    
    cout << "\n--- INFORMACIÓN DEL PRODUCTO ---\n"
         << "Código: " << productos[indice].codigo << "\n"
         << "Nombre: " << productos[indice].nombre << "\n"
         << "Precio: $" << fixed << setprecision(2) << productos[indice].extra << "\n"
         << "Stock: " << productos[indice].stock << "\n";
}

void mostrarResumenVentas() {
    if (!contadores[2]) {
        cout << "\nNo hay ventas registradas.\n";
        return;
    }
    
    mostrarEncabezado("RESUMEN DE VENTAS");
    
    // Estructura para acumular totales
    struct Total { int codigo; float monto; };
    Total porProducto[MAX_PRODUCTOS] = {0}, porCliente[MAX_CLIENTES] = {0};
    float totalGeneral = 0;
    
    // Procesar ventas
    for (int i = 0; i < contadores[2]; i++) {
        int codCliente = ventas[i][0];
        int codProducto = ventas[i][1];
        int cantidad = ventas[i][2];
        
        // Buscar producto para obtener precio
        int idxProd = buscarPorCodigo(productos, contadores[0], codProducto);
        if (idxProd == -1) continue;
        
        float total = cantidad * productos[idxProd].extra;
        totalGeneral += total;
        
        // Acumular por producto
        for (auto& p : porProducto) {
            if (p.codigo == 0) { p.codigo = codProducto; p.monto = total; break; }
            if (p.codigo == codProducto) { p.monto += total; break; }
        }
        
        // Acumular por cliente
        for (auto& c : porCliente) {
            if (c.codigo == 0) { c.codigo = codCliente; c.monto = total; break; }
            if (c.codigo == codCliente) { c.monto += total; break; }
        }
    }
    
    // Mostrar resumen por producto
    auto mostrarResumen = [](const string& titulo, auto& items, int max, auto getNombre) {
        cout << "--- " << titulo << " ---\n"
             << left << setw(10) << "Código" << setw(25) << "Descripción" 
             << setw(15) << "Total" << "\n" << string(50, '-') << '\n';
        
        for (const auto& item : items) {
            if (item.codigo == 0) continue;
            cout << left << setw(10) << item.codigo 
                 << setw(25) << getNombre(item.codigo)
                 << "$" << fixed << setprecision(2) << item.monto << '\n';
        }
    };
    
    mostrarResumen("POR PRODUCTO", porProducto, contadores[0], [](int cod) {
        int idx = buscarPorCodigo(productos, contadores[0], cod);
        return idx != -1 ? productos[idx].nombre : "Producto eliminado";
    });
    
    cout << '\n';
    
    mostrarResumen("POR CLIENTE", porCliente, contadores[1], [](int cod) {
        int idx = buscarPorCodigo(clientes, contadores[1], cod);
        return idx != -1 ? clientes[idx].nombre : "Cliente eliminado";
    });
    
    cout << '\n' << string(50, '-') << '\n'
         << "TOTAL GENERAL: $" << fixed << setprecision(2) << totalGeneral << '\n';
}