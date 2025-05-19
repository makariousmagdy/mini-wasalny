#ifndef GUI1_H
#define GUI1_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "Graph.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GUI1Class; }
QT_END_NAMESPACE

class GUI1 : public QMainWindow
{
    Q_OBJECT

public:
    GUI1(QWidget* parent = nullptr);
    ~GUI1();

private slots:

    void showHomePage();
    void showCitiesPage();
    void showRoadsPage();
    void showDisplayPage();
    void showSearchPage();

    void addCity();
    void updateCity();
    void deleteCity();
    void refreshCitiesList();

    void addRoad();
    void updateRoad();
    void deleteRoad();
    void refreshRoadsTable();

    void displayFullGraph();
    void displayCityNeighbors();

    void findPathDijkstra();
    void findPathDFS();
    void findPathBFS();

    void saveData();
    void loadData();

private:
    Ui::GUI1Class* ui;
    Graph graph;
    QGraphicsScene* scene;
    QGraphicsView* view;

    void setupConnections();
    void populateCitiesComboBoxes();
};
#endif 