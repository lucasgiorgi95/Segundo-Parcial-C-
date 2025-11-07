#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>
#include <vector>

using namespace std;

// ========== VARIABLES GLOBALES ==========
// Vectores para productos
int codProductos[50], stock[50], contadorProductos = 0;
float precios[50];
string nombres[50];

// Vectores para clientes
int codClientes[50], contadorClientes = 0, siguienteCodigoCliente = 1;
string nombresClientes[50], tipoCliente[50]; // "mayorista" o "minorista"

// Matriz para ventas [codCliente, codProducto, cantidad]
int ventas[100][3], contadorVentas = 0;

// Matrices auxiliares
float totalPorProducto[50][2],  // [codProducto, total vendido]
      totalPorCliente[50][2],   // [codCliente, total comprado]
      historialPrecios[50][12]; // Simulación de precios históricos

// ========== IMPLEMENTACIÓN DE FUNCIONES ==========

// Helper: convierte string a minusculas
string toLower(const string &s) {
    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// Función para inicializar datos de ejemplo
void inicializarDatos() {
    for(int i = 0; i < 50; i++) {
        totalPorProducto[i][0] = totalPorProducto[i][1] = 0;
        totalPorCliente[i][0] = totalPorCliente[i][1] = 0;
        for(int j = 0; j < 12; j++) historialPrecios[i][j] = 0;
    }
}

// Función para mostrar el menú principal
void mostrarMenu() {
    cout << "========================================\n"
         << "   SISTEMA DE GESTION - DISTRIBUIDORA\n"
         << "========================================\n"
         << "1. Registrar Productos\n"
         << "2. Registrar Clientes\n"
         << "3. Realizar Venta\n"
         << "4. Consultar Stock\n"
         << "5. Buscar Producto\n"
         << "6. Generar Resumen de Ventas\n"
         << "0. Salir\n========================================\n";
}

// Función para registrar nuevos productos
void registrarProductos() {
    if(contadorProductos >= 50) {
        cout << "\nNo se pueden registrar mas productos. Limite alcanzado.\n";
        return;
    }
    
    cout << "\n--- REGISTRAR PRODUCTO ---\n";
    
    // Solicitar código de producto
    int codigoProd;
    while(true) {
        cout << "Ingrese codigo de producto: ";
        if(!(cin >> codigoProd)) {
            cout << "Entrada invalida. Ingrese un numero entero para el codigo.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Verificar si el código ya existe
        bool existe = false;
        for(int i = 0; i < contadorProductos; i++) {
            if(codProductos[i] == codigoProd) {
                existe = true;
                break;
            }
        }

        if(existe) {
            cout << "Error: El codigo de producto ya existe. Intente con otro.\n";
            continue;
        }

        codProductos[contadorProductos] = codigoProd;
        break;
    }

    // Limpiar buffer antes de getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Solicitar nombre del producto
    cout << "Ingrese nombre del producto: ";
    getline(cin, nombres[contadorProductos]);
    
    // Solicitar precio unitario
    cout << "Ingrese precio unitario: $";
    while(true) {
        if(!(cin >> precios[contadorProductos])) {
            cout << "Entrada invalida. Ingrese un numero para el precio.\n";
             
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ingrese precio unitario: $";
            continue;
        }
        if(precios[contadorProductos] <= 0) {
            cout << "Error: El precio debe ser mayor a 0. Intente nuevamente: $";
            continue;
        }
        break;
    }
    
    // Solicitar stock inicial
    while(true) {
        cout << "Ingrese stock disponible: ";
        if(!(cin >> stock[contadorProductos])) {
            cout << "Entrada invalida. Ingrese un numero entero para el stock.\n";
             
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if(stock[contadorProductos] < 0) {
            cout << "Error: El stock no puede ser negativo. Intente nuevamente.\n";
            continue;
        }
        break;
    }
    
    contadorProductos++;
    cout << "\n¡Producto registrado exitosamente!\n";
}

// Función para registrar nuevos clientes
void registrarClientes() {
    if(contadorClientes >= 50) {
        cout << "\nNo se pueden registrar mas clientes. Limite alcanzado.\n";
        return;
    }
    
    cout << "\n--- REGISTRAR CLIENTE ---\n";
    
    // Asignar codigo automaticamente
    codClientes[contadorClientes] = siguienteCodigoCliente++;

    // Limpiar buffer antes de getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Solicitar nombre del cliente
    cout << "Ingrese nombre del cliente: ";
    getline(cin, nombresClientes[contadorClientes]);

    // Solicitar tipo de cliente
    int tipo;
    while(true) {
        cout << "Tipo de cliente:\n";
        cout << "1. Mayorista\n";
        cout << "2. Minorista\n";
        cout << "Seleccione opcion: ";
        if(!(cin >> tipo)) {
            cout << "Entrada invalida. Ingrese 1 o 2.\n";
             
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if(tipo == 1) {
            tipoCliente[contadorClientes] = "Mayorista";
            break;
        } else if(tipo == 2) {
            tipoCliente[contadorClientes] = "Minorista";
            break;
        } else {
            cout << "Opcion invalida. Ingrese 1 o 2.\n";
            continue;
        }
    }

    cout << "\nCliente registrado con codigo: " << codClientes[contadorClientes] << "\n";
    
    contadorClientes++;
    cout << "\n¡Cliente registrado exitosamente!\n";
}

// Función para mostrar el stock de todos los productos
void mostrarStock() {
    if(contadorProductos == 0) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    cout << "\n--- STOCK DE PRODUCTOS ---\n\n";
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(12) << "Precio" 
         << setw(10) << "Stock" << "\n";
    cout << string(57, '-') << "\n";
    
    for(int i = 0; i < contadorProductos; i++) {
        cout << left << setw(10) << codProductos[i]
             << setw(25) << nombres[i];
        cout << "$" << setw(11) << fixed << setprecision(2) << precios[i];
        cout << setw(10) << right << stock[i] << "\n";
        cout << left; // restore
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
    
    int indice = -1;
    for(int i = 0; i < contadorProductos; i++) {
        if(codProductos[i] == codigo) {
            indice = i;
            break;
        }
    }
    
    if(indice == -1) {
        cout << "\nProducto no encontrado.\n";
        return;
    }
    
    cout << "\n--- INFORMACION DEL PRODUCTO ---\n";
    cout << "Codigo: " << codProductos[indice] << "\n";
    cout << "Nombre: " << nombres[indice] << "\n";
    cout << "Precio: $" << fixed << setprecision(2) << precios[indice] << "\n";
    cout << "Stock: " << stock[indice] << "\n";
}

// Función para realizar una venta
void realizarVenta() {
    if(contadorProductos == 0) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }
    
    if(contadorClientes == 0) {
        cout << "\nNo hay clientes registrados.\n";
        return;
    }
    
    if(contadorVentas >= 100) {
        cout << "\nNo se pueden registrar mas ventas. Limite alcanzado.\n";
        return;
    }
    
    cout << "\n--- REALIZAR VENTA ---\n";
    
    int codProducto, cantidad;
    int indiceCliente = -1, indiceProducto = -1;

    // Solicitar nombre de cliente (buscar por nombre)
    cout << "Ingrese nombre del cliente: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string nombreBuscado;
    getline(cin, nombreBuscado);

    string nombreLower = toLower(nombreBuscado);

    // Buscar coincidencias (case-insensitive)
    vector<int> coincidencias;
    for(int i = 0; i < contadorClientes; i++) {
        if(toLower(nombresClientes[i]).find(nombreLower) != string::npos) {
            coincidencias.push_back(i);
        }
    }

    if(coincidencias.empty()) {
        cout << "Error: Cliente no encontrado.\n";
        return;
    } else if(coincidencias.size() == 1) {
        indiceCliente = coincidencias[0];
    } else {
        cout << "Se encontraron varias coincidencias:\n";
        for(size_t k = 0; k < coincidencias.size(); k++) {
            int idx = coincidencias[k];
            cout << k+1 << ". " << nombresClientes[idx] << " (Codigo: " << codClientes[idx] << ")\n";
        }
        int sel = 0;
        while(true) {
            cout << "Seleccione el numero correspondiente al cliente: ";
            if(!(cin >> sel)) {
                cout << "Entrada invalida.\n";
                 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if(sel < 1 || sel > (int)coincidencias.size()) {
                cout << "Seleccion fuera de rango.\n";
                continue;
            }
            indiceCliente = coincidencias[sel-1];
            break;
        }
    }

    cout << "Cliente: " << nombresClientes[indiceCliente] 
         << " (" << tipoCliente[indiceCliente] << ") Codigo: " << codClientes[indiceCliente] << "\n\n";
    
    // Solicitar código de producto
    cout << "Ingrese codigo de producto: ";
    if(!(cin >> codProducto)) {
        cout << "Entrada invalida. Operacion cancelada.\n";
         
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    
    // Buscar producto
    for(int i = 0; i < contadorProductos; i++) {
        if(codProductos[i] == codProducto) {
            indiceProducto = i;
            break;
        }
    }
    
    if(indiceProducto == -1) {
        cout << "Error: Producto no encontrado.\n";
        return;
    }
    
    cout << "Producto: " << nombres[indiceProducto] << "\n";
    cout << "Precio unitario: $" << fixed << setprecision(2) 
         << precios[indiceProducto] << "\n";
    cout << "Stock disponible: " << stock[indiceProducto] << "\n\n";
    
    // Solicitar cantidad
    cout << "Ingrese cantidad a vender: ";
    if(!(cin >> cantidad)) {
        cout << "Entrada invalida. Operacion cancelada.\n";
         
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    
    // Verificar stock disponible
    if(cantidad > stock[indiceProducto]) {
        cout << "Error: Stock insuficiente.\n";
        return;
    }
    
    if(cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0.\n";
        return;
    }
    
    // Registrar venta en matriz (guardar el codigo real del cliente)
    ventas[contadorVentas][0] = codClientes[indiceCliente];
    ventas[contadorVentas][1] = codProducto;
    ventas[contadorVentas][2] = cantidad;
    contadorVentas++;
    
    // Descontar stock
    stock[indiceProducto] -= cantidad;
    
    // Calcular total de la venta
    float total = cantidad * precios[indiceProducto];
    
    cout << "\n--- RESUMEN DE VENTA ---\n";
    cout << "Total: $" << fixed << setprecision(2) << total << "\n";
    cout << "Stock restante: " << stock[indiceProducto] << "\n";
    cout << "\n¡Venta registrada exitosamente!\n";
}

// Función para generar resumen de ventas
void mostrarResumen() {
    if(contadorVentas == 0) {
        cout << "\nNo hay ventas registradas.\n";
        return;
    }
    
    // Inicializar matrices auxiliares
    for(int i = 0; i < 50; i++) {
        totalPorProducto[i][0] = 0;
        totalPorProducto[i][1] = 0;
        totalPorCliente[i][0] = 0;
        totalPorCliente[i][1] = 0;
    }
    
    // Calcular totales
    for(int i = 0; i < contadorVentas; i++) {
        int codCliente = ventas[i][0];
        int codProducto = ventas[i][1];
        int cantidad = ventas[i][2];
        
        // Buscar precio del producto
        float precio = 0;
        for(int j = 0; j < contadorProductos; j++) {
            if(codProductos[j] == codProducto) {
                precio = precios[j];
                break;
            }
        }
        
        float total = cantidad * precio;
        
        // Acumular por producto
        bool productoEncontrado = false;
        for(int j = 0; j < 50; j++) {
            if(totalPorProducto[j][0] == codProducto) {
                totalPorProducto[j][1] += total;
                productoEncontrado = true;
                break;
            }
            if(totalPorProducto[j][0] == 0 && !productoEncontrado) {
                totalPorProducto[j][0] = codProducto;
                totalPorProducto[j][1] = total;
                break;
            }
        }
        
        // Acumular por cliente
        bool clienteEncontrado = false;
        for(int j = 0; j < 50; j++) {
            if(totalPorCliente[j][0] == codCliente) {
                totalPorCliente[j][1] += total;
                clienteEncontrado = true;
                break;
            }
            if(totalPorCliente[j][0] == 0 && !clienteEncontrado) {
                totalPorCliente[j][0] = codCliente;
                totalPorCliente[j][1] = total;
                break;
            }
        }
    }
    
    // Mostrar resumen por producto
    cout << "\n--- RESUMEN DE VENTAS POR PRODUCTO ---\n\n";
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(15) << "Total Vendido" << "\n";
    cout << string(50, '-') << "\n";
    
    for(int i = 0; i < 50; i++) {
        if(totalPorProducto[i][0] != 0) {
            int cod = totalPorProducto[i][0];
            string nombre = "";
            
            for(int j = 0; j < contadorProductos; j++) {
                if(codProductos[j] == cod) {
                    nombre = nombres[j];
                    break;
                }
            }
            
            cout << left << setw(10) << cod
                 << setw(25) << nombre
                 << "$" << fixed << setprecision(2) 
                 << totalPorProducto[i][1] << "\n";
        }
    }
    
    // Mostrar resumen por cliente
    cout << "\n--- RESUMEN DE VENTAS POR CLIENTE ---\n\n";
    cout << left << setw(10) << "Codigo" 
         << setw(25) << "Nombre" 
         << setw(15) << "Total Comprado" << "\n";
    cout << string(50, '-') << "\n";
    
    for(int i = 0; i < 50; i++) {
        if(totalPorCliente[i][0] != 0) {
            int cod = totalPorCliente[i][0];
            string nombre = "";
            
            for(int j = 0; j < contadorClientes; j++) {
                if(codClientes[j] == cod) {
                    nombre = nombresClientes[j];
                    break;
                }
            }
            
            cout << left << setw(10) << cod
                 << setw(25) << nombre
                 << "$" << fixed << setprecision(2) 
                 << totalPorCliente[i][1] << "\n";
        }
    }
    
    // Calcular total general
    float totalGeneral = 0;
    for(int i = 0; i < 50; i++) totalGeneral += totalPorProducto[i][1];
    
    cout << "\n" << string(50, '-') << "\nTOTAL GENERAL DE VENTAS: $" 
         << fixed << setprecision(2) << totalGeneral << "\n";
}

// ========== FUNCIÓN PRINCIPAL ==========
int main() {
    int opcion;
    
    inicializarDatos();
    
    do {
        mostrarMenu();
        cout << "\nIngrese una opcion: ";
        if(!(cin >> opcion)) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcion invalida. Intente nuevamente.\n";
            opcion = -1;
        }
        
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
                cout << "\n¡Gracias por usar el sistema!\n";
                break;
            default:
                cout << "\nOpcion invalida. Intente nuevamente.\n";
        }
        
        if(opcion != 0) {
            // Limpiar cualquier resto en el buffer y esperar Enter
             
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
        
    } while(opcion != 0);
    
    return 0;
}