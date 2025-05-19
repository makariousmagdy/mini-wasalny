#include"Graph.h"
#include <queue>
#include <unordered_map>
#include <fstream>
#include <string>
#include <limits>
#include <stack>
#include <iterator>
#include <list>
#include <iostream>
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QPen>
#include <QBrush>
#include <QColor>
#include<string>

using namespace std;
const int INF = numeric_limits<int>::max();

void Graph::addVertex(string newVertex) {
    if (checkVertexExistByName(newVertex)) {
        cout << "this vertex is found" << endl;
    }
    else {
        Vertex v(newVertex);
        vertices[newVertex] = v;
    }
}

void Graph::updateVertex(string oldName, string newName) {
    if (!checkVertexExistByName(oldName)) {
        cout << "City " << oldName << " doesn't exist!" << endl;
        return;
    }
    if (checkVertexExistByName(newName)) {
        cout << "City " << newName << " already exists!" << endl;
        return;
    }

    Vertex updatedVertex = vertices[oldName];
    updatedVertex.setStateName(newName);

    list<Edges> oldEdges = updatedVertex.getEdges();
    updatedVertex.edgeList.clear();

    for (Edges edge : oldEdges) {
        Edges updatedEdge(newName, edge.getVertexName2(), edge.getWeight());
        updatedVertex.addEdge(updatedEdge);
    }

    vertices[newName] = updatedVertex;

    for (auto& pair : vertices) {
        if (pair.first != newName && pair.first != oldName) {
            list<Edges>& edges = pair.second.edgeList;
            list<Edges> updatedEdges;

            for (Edges& edge : edges) {
                if (edge.getVertexName2() == oldName) {
                    Edges updatedEdge(pair.first, newName, edge.getWeight());
                    updatedEdges.push_back(updatedEdge);
                }
                else {
                    updatedEdges.push_back(edge);
                }
            }

            pair.second.edgeList = updatedEdges;
        }
    }

    vertices.erase(oldName);
    cout << "City updated from " << oldName << " to " << newName << " successfully!" << endl;
}

void Graph::deleteVertex(string name) {
    if (!checkVertexExistByName(name)) {
        cout << "City " << name << " doesn't exist!" << endl;
        return;
    }

    vertices.erase(name);

    for (auto& pair : vertices) {
        list<Edges>& edges = pair.second.edgeList;
        edges.remove_if([name](Edges& edge) {
            return edge.getVertexName2() == name;
            });
    }

    cout << "City " << name << " and all its connections deleted successfully!" << endl;
}





void Graph::addEdge(string sourceName, string destinationName, int weight) {
    if (!checkVertexExistByName(sourceName) || !checkVertexExistByName(destinationName)) {
        cout << "one or both cities are not in the graph" << endl;
        return;
    }

    Edges e1(sourceName, destinationName, weight);
    vertices[sourceName].addEdge(e1);
    cout << "edge has been added succesfully from" << sourceName << "to" << destinationName << "and its lenght is" << weight << "km" << endl;
}

void Graph::updateEdge(string sourceName, string destinationName, int newWeight) {
    list<Edges>::iterator it;
    for (it = vertices[sourceName].edgeList.begin(); it != vertices[sourceName].edgeList.end(); it++) {
        if (it->vertexName1 == sourceName && it->vertexName2 == destinationName) {
            it->Weight = newWeight;
        }
    }
}

void Graph::deleteEdge(string sourceName, string destinationName) {
    list<Edges>::iterator it;
    for (it = vertices[sourceName].edgeList.begin(); it != vertices[sourceName].edgeList.end(); it++) {
        if (it->vertexName1 == sourceName && it->vertexName2 == destinationName) {
            vertices[sourceName].edgeList.erase(it);
            cout << "edge deleted successfully" << endl;
            break;
        }
        else
        {
            cout << "this edge is not found" << endl;
        }
    }
}





bool Graph::checkVertexExistByName(string name) {
    return vertices.find(name) != vertices.end();
}

bool Graph::checkIfNeighbors(string sourceName, string destinationName) {
    if (!checkVertexExistByName(sourceName) || !checkVertexExistByName(destinationName)) {
        return false;
    }

    for (Edges& edge : vertices[sourceName].edgeList) {
        if (edge.getVertexName2() == destinationName) {
            return true;
        }
    }

    return false;
}





vector<string> Graph::getAllNeighbors(string name) {
    vector<string> neighbors;

    if (!checkVertexExistByName(name)) {
        return neighbors;
    }

    for (Edges& edge : vertices[name].edgeList) {
        neighbors.push_back(edge.getVertexName2());
    }

    return neighbors;
}

void Graph::printAllNeighbors(string name) {
    if (!checkVertexExistByName(name)) {
        cout << "City " << name << " doesn't exist!" << endl;
        return;
    }

    vector<string> neighbors = getAllNeighbors(name);

    if (neighbors.empty()) {
        cout << "City " << name << " has no connections." << endl;
        return;
    }

    cout << "Connections from " << name << ":" << endl;
    for (Edges& edge : vertices[name].edgeList) {
        cout << "  -> " << edge.getVertexName2() << " (Distance: " << edge.getWeight() << "km)" << endl;
    }
}

void Graph::printGraph() {
    if (vertices.empty()) {
        cout << "Graph is empty." << endl;
        return;
    }

    cout << "\nGraph Contents:" << endl;
    cout << "---------------" << endl;

    for (auto vertexItem : vertices) {
        string name = vertexItem.first;
        Vertex vertex = vertexItem.second;

        cout << "City " << name << " (" << vertex.getStatename() << "): ";

        list<Edges> edges = vertex.getEdges();
        if (edges.empty()) {
            cout << "No connections";
        }
        else {
            for (Edges edge : edges) {
                cout << edge.getVertexName2() << "( Destince: " << edge.getWeight() << "km ) ";
            }
        }
        cout << endl;
    }
}

void Graph::visualizeGraph(QGraphicsScene* scene) {
    if (vertices.empty()) {
        qDebug() << "Graph is empty.";
        return;
    }

    scene->clear();

    const int centerX = 400;
    const int centerY = 300;
    const int radius = 360;
    const int nodeRadius = 70;

    unordered_map<string, QPointF> nodePositions;
    double angleStep = 2 * M_PI / vertices.size();
    double currentAngle = 0;

    for (const auto& vertexPair : vertices) {
        string cityName = vertexPair.first;

        double x = centerX + radius * cos(currentAngle);
        double y = centerY + radius * sin(currentAngle);
        nodePositions[cityName] = QPointF(x, y);

        QGraphicsEllipseItem* node = scene->addEllipse(
            x - nodeRadius, y - nodeRadius,
            nodeRadius * 2, nodeRadius * 2,
            QPen(Qt::white), QBrush(Qt::lightGray));

        QGraphicsTextItem* text = scene->addText(QString::fromStdString(cityName));
        text->setPos(x - text->boundingRect().width() / 2,
            y - text->boundingRect().height() / 2);

        QFont cityFont("Arial", 20, QFont::Bold);
        QColor cityColor(0, 0, 0);
        text->setFont(cityFont);
        text->setDefaultTextColor(cityColor);
        currentAngle += angleStep;
    }

    for (const auto& vertexPair : vertices) {
        string source = vertexPair.first;
        QPointF sourcePos = nodePositions[source];

        for (const auto& edge : vertexPair.second.edgeList) {
            string dest = edge.vertexName2;
            QPointF destPos = nodePositions[dest];

            QGraphicsLineItem* line = scene->addLine(
                QLineF(sourcePos, destPos),
                QPen(Qt::blue, 2));

            QPointF midPoint = (sourcePos + destPos) / 2;
            QGraphicsTextItem* weightText = scene->addText(
                QString::number(edge.Weight));
            weightText->setPos(midPoint);
            weightText->setDefaultTextColor(Qt::red);
            weightText->setFont(QFont("Arial", 20));
        }
    }
}




string Graph::BFS(string name)
{
    string result;
    if (!checkVertexExistByName(name)) {
        return "City not found!";
    }
    list <Edges> tmpedge = vertices[name].edgeList;
    unordered_map<string, bool> visiting_state;
    for (auto& pair : vertices) {
        visiting_state[pair.first] = false;
    }
    queue<string> cities;
    cities.push(name);
    visiting_state[name] = true;
    result += "BFS traversal starting from  " + name +  ":\n";
    while (true) {
        while (!tmpedge.empty()) {
            if (visiting_state[tmpedge.front().vertexName2] == true) {
                tmpedge.pop_front();
                continue;
            }
            cities.push(tmpedge.front().vertexName2);
            visiting_state[tmpedge.front().vertexName2] = true;
            tmpedge.pop_front();
        }
        result += cities.front() + "  ";
        cities.pop();
        if (!cities.empty()) {
            tmpedge = vertices[cities.front()].edgeList;
        }
        else
            break;
    }
    return result;
}

string Graph::DFS(string current_city)
{
    unordered_map<string, bool> visited;
    stack<pair<string, vector<string>>> city;
    vector<string> allWays;

    for (const auto& pair : vertices) {
        visited[pair.first] = false;
    }

    city.push({ current_city, { current_city } });

    while (!city.empty())
    {
        auto top = city.top();
        city.pop();

        string current = top.first;
        vector<string> path = top.second;

        visited[current] = true;

        bool unvisited = false;
        auto& Edges = vertices[current].edgeList;

        for (const auto& edge : Edges)
        {
            string branch = edge.vertexName2;
            if (!visited[branch]) {
                unvisited = true;
                vector<string> new_path = path;
                new_path.push_back(branch);
                city.push({ branch, new_path });
            }
        }

        if (!unvisited) {
            string way;
            for (size_t i = 0; i < path.size(); ++i)
            {
                way += path[i];
                if (i != path.size() - 1)
                    way += " -> ";
            }
            allWays.push_back(way);
        }
    }

    string result;
    for (const string& way : allWays)
    {
        result += way + "\n";
    }
    return result;
}

string Graph::dijkstra(const string & startcity, const string & endcity)
{
    string result = "";
    if (checkVertexExistByName(startcity) == false || checkVertexExistByName(endcity) == false) {
        return "Start or end city does not exist.";
    }
    vector<int> dist(vertices.size(), INF);
    vector <int> parent(vertices.size(), -1);
    priority_queue <pair<int, int>, vector <pair<int, int> >, greater<pair<int, int>>> pq;

    unordered_map<string, int> city_index;
    unordered_map<int, string> reversed_city_index;
    int c = 0;
    for (const auto& pair : vertices) {
        string name = pair.first;
        city_index.insert({ name, c });
        reversed_city_index.insert({ c, name });
        c++;
    }
    int start = city_index[startcity];
    int end = city_index[endcity];
    dist[start] = 0;
    pq.push({ 0, start });
    while (!pq.empty()) {
        int current_city_index = pq.top().second;
        int currDist = pq.top().first;
        pq.pop();

        if (currDist > dist[current_city_index]) {
            continue;
        }
        if (current_city_index == end) {
            break;
        }
        for (auto& edge : vertices[reversed_city_index[current_city_index]].edgeList)
        {
            string name = edge.getVertexName2();
            int neighbor_city_index = city_index[name];
            int weight = edge.getWeight();
            if ((dist[current_city_index] + weight) < dist[neighbor_city_index]) {
                dist[neighbor_city_index] = dist[current_city_index] + weight;
                parent[neighbor_city_index] = current_city_index;
                pq.push({ dist[neighbor_city_index], neighbor_city_index });
            }
        }
    }
    {
        if (dist[end] == INF) {
            result += "there is no path between " + startcity + " and " + endcity + '\n';
            return result;
        }
    }
    vector<int>path;
    int currcity = end;
    while (currcity != -1) {
        path.push_back(currcity);
        currcity = parent[currcity];
    }
    result = result + "Shortest path between " + startcity + " and " + endcity + " is: " + to_string(dist[end]) + "Km" + '\n';
    reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); i++) {
        result += reversed_city_index[path[i]] + " ";
        if (i != path.size() - 1) result = result + "--->";
    }
    return result;
}



void Graph::saveData(unordered_map<string, Vertex> vertices) {
    fstream data;
    data.open("vertex.txt", ios::out);
    for (const auto& pair : vertices) {
        data << pair.second.State_name << endl;
        for (const auto& edge : pair.second.edgeList) {
            data << edge.vertexName2 << "--" << edge.Weight << " ";
        }
        data << endl;
    }
    data.close();
}

unordered_map<string, Vertex> Graph::readData() {
    unordered_map<string, Vertex> ver;
    fstream data;
    data.open("vertex.txt", ios::in);
    string name;
    string source;
    string neighbour;
    string na;
    string w;
    while (getline(data, source)) {
        Vertex v(source);
        getline(data, neighbour);
        int i = 0;
        name = "";
        while (neighbour.length() != 0) {
            i = 0;
            name = "";
            w = "";
            while (neighbour[i] != '-') {
                name += neighbour[i];
                i++;
            }
            i++;
            neighbour.erase(0, i + 1);
            i = 0;
            int pos = neighbour.find(' ');
            while (i != pos) {
                w += neighbour[i];
                i++;
            }
            Edges e(source, name, stoi(w));
            v.addEdge(e);
            neighbour.erase(0, i + 1);
        }
        name = "";
        w = "";
        ver[source] = v;
    }
    return ver;
}
