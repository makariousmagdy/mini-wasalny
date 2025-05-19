#include "GUI1.h"
#include "ui_GUI1.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTableWidgetItem>
#include <QFile>

GUI1::GUI1(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::GUI1Class)
    , scene(new QGraphicsScene(this))
    , view(new QGraphicsView(scene))
{
    ui->setupUi(this);
    ui->verticalLayout_5->insertWidget(1, view);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);




    graph = Graph();

    setupConnections();
    populateCitiesComboBoxes();
    refreshCitiesList();
    refreshRoadsTable();

    ui->stackedWidget->setCurrentIndex(0);
}

GUI1::~GUI1()
{
    delete ui;
}

void GUI1::setupConnections()
{
  
    connect(ui->btnCities, &QPushButton::clicked, this, &GUI1::showCitiesPage);
    connect(ui->btnRoads, &QPushButton::clicked, this, &GUI1::showRoadsPage);
    connect(ui->btnDisplay, &QPushButton::clicked, this, &GUI1::showDisplayPage);
    connect(ui->btnSearch, &QPushButton::clicked, this, &GUI1::showSearchPage);

    connect(ui->btnHome, &QPushButton::clicked, this, &GUI1::showHomePage);
    connect(ui->btnHome_2, &QPushButton::clicked, this, &GUI1::showHomePage);
    connect(ui->btnHome_3, &QPushButton::clicked, this, &GUI1::showHomePage);
    connect(ui->btnHome_4, &QPushButton::clicked, this, &GUI1::showHomePage);

    connect(ui->btnAddCity, &QPushButton::clicked, this, &GUI1::addCity);
    connect(ui->btnUpdateCity, &QPushButton::clicked, this, &GUI1::updateCity);
    connect(ui->btnDeleteCity, &QPushButton::clicked, this, &GUI1::deleteCity);
    connect(ui->btnRefreshCities, &QPushButton::clicked, this, &GUI1::refreshCitiesList);

    connect(ui->btnAddRoad, &QPushButton::clicked, this, &GUI1::addRoad);
    connect(ui->btnUpdateRoad, &QPushButton::clicked, this, &GUI1::updateRoad);
    connect(ui->btnDeleteRoad, &QPushButton::clicked, this, &GUI1::deleteRoad);
    connect(ui->btnRefreshRoads, &QPushButton::clicked, this, &GUI1::refreshRoadsTable);

    connect(ui->btnDisplayFull, &QPushButton::clicked, this, &GUI1::displayFullGraph);
    connect(ui->btnDisplayNeighbors, &QPushButton::clicked, this, &GUI1::displayCityNeighbors);

    connect(ui->btnDijkstra, &QPushButton::clicked, this, &GUI1::findPathDijkstra);
    connect(ui->btnDFS, &QPushButton::clicked, this, &GUI1::findPathDFS);
    connect(ui->btnBFS, &QPushButton::clicked, this, &GUI1::findPathBFS);

    connect(ui->actionSave, &QAction::triggered, this, &GUI1::saveData);
    connect(ui->actionLoad, &QAction::triggered, this, &GUI1::loadData);

    connect(ui->btnDisplayFull, &QPushButton::clicked, this, &GUI1::displayFullGraph);
    connect(ui->btnDisplayNeighbors, &QPushButton::clicked, this, &GUI1::displayCityNeighbors);

}


void GUI1::showHomePage() { ui->stackedWidget->setCurrentIndex(0); }
void GUI1::showCitiesPage() { ui->stackedWidget->setCurrentIndex(1); }
void GUI1::showRoadsPage() { ui->stackedWidget->setCurrentIndex(2); }
void GUI1::showDisplayPage() { ui->stackedWidget->setCurrentIndex(3); }
void GUI1::showSearchPage() { ui->stackedWidget->setCurrentIndex(4); }

void GUI1::addCity()
{
    QString cityName = ui->txtCityName->text().trimmed();
    if (cityName.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please enter city name");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    std::string stdCityName = cityName.toStdString();
    if (graph.checkVertexExistByName(stdCityName)) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("City already exists!");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);

        msgBox.exec();
        return;
    }

    graph.addVertex(stdCityName);
    populateCitiesComboBoxes();
    refreshCitiesList();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("City added successfully");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
                         "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
    ui->txtCityName->clear();
}

void GUI1::updateCity()
{
    QListWidgetItem* selectedItem = ui->listCities->currentItem();
    if (!selectedItem) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select a city to update");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString oldName = selectedItem->text();
    QString newName = ui->txtCityName->text().trimmed();

    if (newName.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please enter new name");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    graph.updateVertex(oldName.toStdString(), newName.toStdString());
    populateCitiesComboBoxes();
    refreshCitiesList();
    refreshRoadsTable();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("City updated successfully");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
        "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
    ui->txtCityName->clear();
}

void GUI1::deleteCity()
{
    QListWidgetItem* selectedItem = ui->listCities->currentItem();
    if (!selectedItem) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select a city to delete");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString cityName = selectedItem->text();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Confirm Delete");
    msgBox.setText("Are you sure you want to delete '" + cityName + "' and all its connections?");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
                         "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    QPushButton* yesButton = msgBox.addButton(QMessageBox::Yes);
    QPushButton* noButton = msgBox.addButton(QMessageBox::No);
    yesButton->setText("Delete");
    noButton->setText("Cancel");
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        graph.deleteVertex(cityName.toStdString());
        populateCitiesComboBoxes();
        refreshCitiesList();
        refreshRoadsTable();
        QMessageBox infoBox(this);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.setWindowTitle("Success");
        infoBox.setText("City deleted successfully");
        infoBox.setStyleSheet("QMessageBox { background-color: white; } "
                              "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        infoBox.addButton(QMessageBox::Ok);
        infoBox.exec();
    }
}

void GUI1::refreshCitiesList()
{
    ui->listCities->clear();
    int i = 0;
    for (const auto& city : graph.vertices) {
        ui->listCities->addItem(QString::fromStdString(city.first));
    }
}

void GUI1::addRoad()
{
    QString source = ui->cmbSourceCity->currentText();
    QString destination = ui->cmbDestinationCity->currentText();
    int distance = ui->spinDistance->value();

    if (source.isEmpty() || destination.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select source and destination cities");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    if (source == destination) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Cannot add road between same city");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
	if (graph.checkIfNeighbors(source.toStdString(), destination.toStdString())) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Road already exists between these cities");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
		return;
	}

    graph.addEdge(source.toStdString(), destination.toStdString(), distance);
    refreshRoadsTable();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("Road added successfully");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
        "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void GUI1::updateRoad()
{
    int selectedRow = ui->tableRoads->currentRow();
    if (selectedRow == -1) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select a road to update");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString source = ui->tableRoads->item(selectedRow, 0)->text();
    QString destination = ui->tableRoads->item(selectedRow, 1)->text();
    int newDistance = ui->spinDistance->value();

    graph.updateEdge(source.toStdString(), destination.toStdString(), newDistance);
    refreshRoadsTable();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("Road updated successfully");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
        "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void GUI1::deleteRoad()
{
    int selectedRow = ui->tableRoads->currentRow();
    if (selectedRow == -1) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select a road to delete");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QString source = ui->tableRoads->item(selectedRow, 0)->text();
    QString destination = ui->tableRoads->item(selectedRow, 1)->text();

    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle("Confirm Delete");
    msgBox.setText("Are you sure you want to delete road between '" + source + "' and '" + destination + "'?");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
                         "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    QPushButton* yesButton = msgBox.addButton(QMessageBox::Yes);
    QPushButton* noButton = msgBox.addButton(QMessageBox::No);
    yesButton->setText("Delete");
    noButton->setText("Cancel");
    msgBox.exec();

    if (msgBox.clickedButton() == yesButton) {
        graph.deleteEdge(source.toStdString(), destination.toStdString());
        refreshRoadsTable();
        QMessageBox infoBox(this);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.setWindowTitle("Success");
        infoBox.setText("Road deleted successfully");
        infoBox.setStyleSheet("QMessageBox { background-color: white; } "
                              "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        infoBox.addButton(QMessageBox::Ok);
        infoBox.exec();
    }
}

void GUI1::refreshRoadsTable()
{
    ui->tableRoads->setRowCount(0);
    ui->tableRoads->setColumnCount(3);
    ui->tableRoads->setHorizontalHeaderLabels({ "Source", "Destination", "Distance (km)" });

    for (const auto& vertexPair : graph.vertices) {
        for (const auto& edge : vertexPair.second.edgeList) {
            int row = ui->tableRoads->rowCount();
            ui->tableRoads->insertRow(row);

            ui->tableRoads->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(edge.vertexName1)));
            ui->tableRoads->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(edge.vertexName2)));
            ui->tableRoads->setItem(row, 2, new QTableWidgetItem(QString::number(edge.Weight)));
        }
    }

    ui->tableRoads->resizeColumnsToContents();
}


void GUI1::displayFullGraph()
{
    graph.visualizeGraph(scene);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    QString result;
    for (const auto& vertexPair : graph.vertices) {
        result += "City: " + QString::fromStdString(vertexPair.first) + "\n";
        result += "Connections:\n";

        if (vertexPair.second.edgeList.empty()) {
            result += "  No connections\n";
        }
        else {
            for (const auto& edge : vertexPair.second.edgeList) {
                result += "  -> " + QString::fromStdString(edge.vertexName2) +
                    " (" + QString::number(edge.Weight) + " km)\n";
            }
        }
        result += "\n";
    }

    ui->txtDisplay->setPlainText(result);
}

void GUI1::displayCityNeighbors()
{
    QString cityName = ui->cmbDisplayCity->currentText();
    if (cityName.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select a city");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

  
    scene->clear();
    const int nodeRadius = 30;
    QPointF center(400, 300);

    QGraphicsEllipseItem* centerNode = scene->addEllipse(
        center.x() - nodeRadius, center.y() - nodeRadius,
        nodeRadius * 2, nodeRadius * 2,
        QPen(Qt::black, 2), QBrush(Qt::green));

    QGraphicsTextItem* centerText = scene->addText(cityName);
    centerText->setPos(center.x() - centerText->boundingRect().width() / 2,
        center.y() - centerText->boundingRect().height() / 2);

    vector<string> neighbors = graph.getAllNeighbors(cityName.toStdString());
    double angleStep = 2 * M_PI / neighbors.size();
    double currentAngle = 0;
    const int neighborRadius = 150;

    for (const string& neighbor : neighbors) {
        double x = center.x() + neighborRadius * cos(currentAngle);
        double y = center.y() + neighborRadius * sin(currentAngle);

        QGraphicsEllipseItem* node = scene->addEllipse(
            x - nodeRadius, y - nodeRadius,
            nodeRadius * 2, nodeRadius * 2,
            QPen(Qt::black), QBrush(Qt::lightGray));

        QGraphicsTextItem* text = scene->addText(QString::fromStdString(neighbor));
        text->setPos(x - text->boundingRect().width() / 2,
            y - text->boundingRect().height() / 2);

        scene->addLine(QLineF(center.x(), center.y(), x, y), QPen(Qt::blue, 2));

        currentAngle += angleStep;
    }

    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    QString result = "City: " + cityName + "\n\nNeighbors:\n";
    if (neighbors.empty()) {
        result += "No neighboring cities";
    }
    else {
        for (const string& neighbor : neighbors) {
            result += QString::fromStdString(neighbor) + "\n";
        }
    }
    ui->txtDisplay->setPlainText(result);
}


void GUI1::findPathDijkstra() {
    QString start = ui->cmbStartCity->currentText();
    QString end = ui->cmbEndCity->currentText();

    if (start.isEmpty() || end.isEmpty()) {
        
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select start and end cities");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    string result = graph.dijkstra(start.toStdString(), end.toStdString());
    ui->txtResults->setPlainText(QString::fromStdString(result));
}

void GUI1::findPathDFS() {
    QString start = ui->cmbStartCity->currentText();
    if (start.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select start city");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    string result = graph.DFS(start.toStdString());
    ui->txtResults->setPlainText(QString::fromStdString(result));
}

void GUI1::findPathBFS() {
    QString start = ui->cmbStartCity->currentText();

    if (start.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Error");
        msgBox.setText("Please select start city");
        msgBox.setStyleSheet("QMessageBox { background-color: white; } "
            "QPushButton{ min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    string result = graph.BFS(start.toStdString());
    ui->txtResults->setPlainText(QString::fromStdString(result));
}

void GUI1::saveData()
{
    
    graph.saveData(graph.vertices);
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("Data saved to vertex.txt");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
        "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void GUI1::loadData()
{
    graph.vertices = graph.readData();
    populateCitiesComboBoxes();
    refreshCitiesList();
    refreshRoadsTable();
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Success");
    msgBox.setText("Data loaded from vertex.txt");
    msgBox.setStyleSheet("QMessageBox { background-color: white; } "
        "QPushButton { min-width: 50px; height:20px; font-size: 15px; border-radius: 30px; background-color: #e0e0e0; }");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.exec();
}

void GUI1::populateCitiesComboBoxes()
{
    ui->cmbSourceCity->clear();
    ui->cmbDestinationCity->clear();
    ui->cmbDisplayCity->clear();
    ui->cmbStartCity->clear();
    ui->cmbEndCity->clear();

    for (const auto& city : graph.vertices) {
        QString cityName = QString::fromStdString(city.first);
        ui->cmbSourceCity->addItem(cityName);
        ui->cmbDestinationCity->addItem(cityName);
        ui->cmbDisplayCity->addItem(cityName);
        ui->cmbStartCity->addItem(cityName);
        ui->cmbEndCity->addItem(cityName);
    }
}