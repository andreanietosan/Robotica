#include <iostream>
#include <map>
#include <vector>
using namespace std;

int main() {
    //std::cout << "Hello, World!" << std::endl;

    struct Node{ int id; std::vector<int> links;} ;
    std::map<int, Node> graph;

    for(int i=0; i<100; i++)
    graph.emplace(std::make_pair(i, Node{i, std::vector<int>()}));

    int vecinos;
    srand(time(NULL));
    for(auto &pos : graph) {
        vecinos = rand() % 6;
        for (int j=0; j < vecinos; j++)
            pos.second.links.emplace_back(rand() % 100);

    }

    for(auto &pos : graph) {
        cout << " Mapa:" << pos.first << " Id:" << pos.second.id << endl;

        for (auto i: pos.second.links)
            cout << "Links: " << i << endl;
    }

        return 0;
}
