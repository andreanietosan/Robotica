#include <iostream>
#include <cmath>
using namespace std;

void funcionA(double rad){
    cout<<"Radianes: "<<rad<<endl;
}

int main() {
    //std::cout << "Hello, World!" << std::endl;
    double grados=38.0;
   //primera variante, declaramos una funcion lamda y la llamamos en el metodo que queremos ejecutar
    auto f = [=](){ return (grados*M_PI)/180; };
    funcionA(f());

    //segunda variante, declaramos la funcion lamda en la llamada al metodo como parametro de entrada y se ejecuta
    //en caliente, es decir,
    funcionA([=](){ return (grados*M_PI)/180; }());

    return 0;
}
