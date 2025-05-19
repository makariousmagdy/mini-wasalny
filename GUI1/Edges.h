#include<iostream>
using namespace std;
#pragma once
class Edges
{
public:
    string vertexName1;
    string vertexName2;
    int Weight;

public:
    Edges(string n1, string n2, int weight);
    Edges();
    void setEdgesValues(string n1, string n2, int weight);
    void setWeight(int weight);
    string getVertexName1();
    string getVertexName2();
    int getWeight();
};
