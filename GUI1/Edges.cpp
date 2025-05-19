#include "Edges.h"


Edges::Edges(string n1, string n2, int weight)
    : vertexName1(n1), vertexName2(n2), Weight(weight) {
}

Edges::Edges() : vertexName1(""), vertexName2(""), Weight(0) {}


void Edges::setEdgesValues(string n1, string n2, int weight) {
    vertexName1 = n1;
    vertexName2 = n2;
    Weight = weight;
}
void Edges::setWeight(int weight) {
    Weight = weight;
}


string Edges::getVertexName1() {
    return vertexName1;
}

string Edges::getVertexName2() {
    return vertexName2;
}


int Edges::getWeight() {
    return Weight;
}


