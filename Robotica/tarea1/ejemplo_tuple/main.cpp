#include <iostream>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <string>

int main() {
    //std::cout << "Hello, World!" << std::endl;

    std::vector<std::tuple<std::string, int> > v;

    v.push_back(std::make_tuple("Andrea", 30));
    v.push_back(std::make_tuple("Gonzalo", 100));
    v.push_back(std::make_tuple("Carmen", 25));

    // Mostramos la tupla en orden de inserccion
   /*for (int i = 0; i < v.size(); i++)
        std::cout << std::get<0>(v[i]) << " "
             << std::get<1>(v[i]) << "\n";*/

    //Ordenamos la vector por el criterio de la funcion landa (para string en orden alfabetico < y para el reverso >) y para numeros ascentes < y descendente >
    std::sort(v. begin(), v.end(), [](std::tuple<std::string, int> a, std::tuple<std::string, int> b) {
        return std::get<0>(a) < (std::get<0>(b));
    });

    // Mostramos la tupla
    for (int i = 0; i < v.size(); i++)
    std::cout << std::get<0>(v[i]) << " "
              << std::get<1>(v[i]) << "\n";

    return 0;
}
