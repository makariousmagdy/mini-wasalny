#include<iostream>
#include<list>
#include"Edges.h"
using namespace std;
#pragma once
class Vertex
{
public:
	string State_name;
	list<Edges> edgeList;
	Vertex();
	Vertex(string name);
	void addEdge(Edges& edge);
	string getStatename();
	void setStateName(string name);
	list<Edges>& getEdges();

};