#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

int main() {
    //std::cout << "Hello, World!" << std::endl;

    /////////////////////// VECTORES /////////////////////////////////////////////////
    //creamos el vector numeros con 1000000 posiciones
    std::vector<int> numeros(1000000);

    //rellenamos el vector numeros con numeros randoms
    srand(time(NULL));
    for (int i = 0; i < numeros.size(); i++) {
        numeros[i] = (rand() % 1000000) + 1;
    }

    //ordenamos el array con el operador default que es de menor a mayor
    //std::sort(numeros. begin(), numeros.end());

    //mostramos el array
    /*for (int i=0; i<numeros.size(); i++){
        std::cout<<"Posicion Array:"<<i<<" Numero:"<<numeros[i]<< std::endl;
    }*/

    //mostramos el elemento menor del array (el primero) ya que ha sido ordenado de menor a mayor
    //std::cout << numeros.front() << std::endl;

    //ordenamos el vector y realizamos el comparador con una funcion landa
    std:sort(numeros.begin(), numeros.end(), [](int a, int b) {
        return a < b;
    });

    //mostramos el menor valor del array
    //std::cout << numeros.front() << std::endl;

    //creamos un nuevo array y realizamos una copia de numeros
    std::vector<int> num;
    num = numeros;

    //mostramos si se ha realizado la copia correctamente
    //for (int i = 0; i < 6; i++)
     //   std::cout << "Numeros:" << numeros[0] << " Num:" << num[0] << std::endl;

    //copiar una parte del vector numeros en el vector n
    //std::vector<int> n(numeros.begin()+3, numeros.end()-10);

    //mostramos los tamaños de los vectores y vemos que tiene 13 posiciones menos
    //std::cout<<"Numeros:"<<numeros.size()<<" n:"<<n.size()<<std::endl;

    //para recorrer un vector con auto
    for (auto i: num)
    {
        std::cout << i << ","<<std::endl;
    }
    std::cout << "\n";

    return 0;
}
