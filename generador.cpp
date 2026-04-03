//
// Created by mynorm50 on 1/4/26.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

std::string randomNombre(int i) {
    return "Producto_" + std::to_string(i);
}

std::string randomCategoria() {
    std::string cats[] = {"Lacteos","Frutas","Verduras","Bebidas","Snacks","Abarrotes"};
    return cats[rand() % 6];
}

std::string randomFecha() {
    int year = 2026;
    int month = rand() % 12 + 1;
    int day = rand() % 28 + 1;
    return std::to_string(year) + "-" +
            (month < 10 ? "0" : "") + std::to_string(month) + "-" +
            (day < 10 ? "0" : "") + std::to_string(day);
}

int main() {
    std::ofstream file("prueba_grande.csv");
    file << "codigoBarras,nombre,categoria,fechaVencimiento,marca,precio,stock\n";

    srand(time(0));

    for (int i = 1; i <= 1500; i++) {
        int tipoError = rand() % 10;

        std::string codigo = std::to_string(1000 + i);
        std::string nombre = randomNombre(i);
        std::string categoria = randomCategoria();
        std::string fecha = randomFecha();
        std::string marca = "Marca_" + std::to_string(i);
        std::string precio = std::to_string((rand() % 200) + 1);
        std::string stock = std::to_string((rand() % 100) + 1);

        // Inyectar errores aleatorios
        if (tipoError == 0) nombre = ""; // campo vacío
        if (tipoError == 1) fecha = "2026/99/99"; // fecha inválida
        if (tipoError == 2) precio = "-50"; // precio negativo
        if (tipoError == 3) stock = "-10"; // stock negativo
        if (tipoError == 4) precio = "abc"; // precio texto
        if (tipoError == 5) stock = "xyz"; // stock texto
        if (tipoError == 6) codigo = "1001"; // duplicado intencional

        file << codigo << ","
            << nombre << ","
            << categoria << ","
            << fecha << ","
            << marca << ","
            << precio << ","
            << stock << "\n";
    }

    file.close();
    std::cout << "Archivo prueba_grande.csv generado.\n";
}