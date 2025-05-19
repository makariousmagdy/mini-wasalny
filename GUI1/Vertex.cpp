#include "Vertex.h"

Vertex::Vertex()
{
	State_name = "";

}

Vertex::Vertex(string name)
{
	State_name = name;
}

string Vertex::getStatename()
{
	return State_name;
}
void Vertex::addEdge(Edges& edge) {
	edgeList.push_back(edge);
}

void Vertex::setStateName(string name)
{
	State_name = name;
}

list<Edges>& Vertex::getEdges()
{
	return edgeList;
}