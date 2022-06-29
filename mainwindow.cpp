#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IOCconatiner.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(100, 100, 1000, 600);
    // creating widgets
    btnChangeDirectory = new QPushButton("Change Directory");
    btnPrintChart = new QPushButton("Print Chart");
    chkbxBlackWhiteChart = new QCheckBox("B/w Chart");
    comboboxChartType = new QComboBox();
    comboboxChartType->addItem("Bar");
    comboboxChartType->addItem("Pie");

    // creating layouts
    wrapperLayout = new QHBoxLayout(this);
    fileExplorerLayout = new QVBoxLayout();
    chartLayout = new QVBoxLayout();
    chartWidgetLayout = new QHBoxLayout();

    fileSplitter = new QSplitter();
    chartSplitter = new QSplitter();

    wrapperLayout->addLayout(fileExplorerLayout);
    wrapperLayout->addLayout(chartLayout);

    chartLayout->addLayout(chartWidgetLayout);

    fileExplorerLayout->addWidget(fileSplitter, 1);
    chartLayout->addWidget(chartSplitter);

    chartWidgetLayout->addWidget(comboboxChartType);
    chartWidgetLayout->addWidget(chkbxBlackWhiteChart);
    chartWidgetLayout->addWidget(btnPrintChart);

    // file view setup

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    directoryPath = QDir::currentPath();
    pathLabel = new QLabel();
    pathLabel->setText(directoryPath);
    QModelIndex pathIndex = fileModel->setRootPath(directoryPath);

    tableFileView = new QTableView();
    tableFileView->setModel(fileModel);
    tableFileView->setRootIndex(pathIndex);

    // set selection in table only for one item
    tableFileView->setSelectionMode(QAbstractItemView::SingleSelection);

    // chart view setup
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);

    // placement

    fileSplitter->addWidget(tableFileView);
    chartSplitter->addWidget(chartView);
    fileExplorerLayout->addWidget(btnChangeDirectory, 0, Qt::AlignBottom);
    fileExplorerLayout->addWidget(pathLabel, 0, Qt::AlignBottom);

    //connections
    connect(btnChangeDirectory, SIGNAL(clicked(bool)), this, SLOT(changeDirectory()));
    connect(
                tableFileView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
                this, SLOT(fileSelection(const QItemSelection, const QItemSelection))
                );
    connect(comboboxChartType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChartType()));
    connect(chkbxBlackWhiteChart, SIGNAL(toggled(bool)), this, SLOT(colorSwap()));
}

void MainWindow::changeDirectory() {
    QFileDialog dialogWindow(this);
    dialogWindow.setFileMode(QFileDialog::Directory);
    if (dialogWindow.exec()) {
        directoryPath = dialogWindow.selectedFiles().first();
        pathLabel->setText(directoryPath);
    }
    tableFileView->setRootIndex(fileModel->setRootPath(directoryPath));
}

void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    if (indexes.count() < 1) {
//        tableFileView->selectionModel()->select(first.)
        exceptionCall("Selection Error", "No items has been Selected");
        return;
    }
    filePath = fileModel->filePath(indexes.first());
    if (filePath.endsWith(".json")) {
//        auto* json = new JsonDataStructure();
//        json->getData(filePath);
        iocContainer.RegisterInstance<IDataStructure, JsonDataStructure>();
    }
    else if (filePath.endsWith(".sqlite")) {
//        auto* sql = new SqlDataStructure();
//        sql->getData(filePath);
        iocContainer.RegisterInstance<IDataStructure, SqlDataStructure>();
    }
    else {
        exceptionCall("Wrong file format", "Please select .json or .sqlite file");
        return;
    }
    if (comboboxChartType->currentText() == "Pie") {
        iocContainer.RegisterInstance<IChart, PieChart>();
        isChartActive = true;
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
        isChartActive = true;
    }
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::changeChartType() {
    if (comboboxChartType->currentText() == "Pie") {
        iocContainer.RegisterInstance<IChart, PieChart>();
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
    }
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::colorSwap() {
    if (isChartActive) drawChart();
}

void MainWindow::printChart() {

}

void MainWindow::drawChart() {
    auto chart = iocContainer.GetObject<IChart>();
    auto dataStructure = iocContainer.GetObject<IDataStructure>();
    QList<Data> items = dataStructure->getData(filePath);
    chart->recreateChart(items, chkbxBlackWhiteChart->isChecked());
    chartView->setChart(chart->getChart());
}

void MainWindow::exceptionCall(QString title, QString message) {
    QMessageBox *messageBox = new QMessageBox();
    messageBox->setWindowTitle(title);
    messageBox->setText(message);
    messageBox->exec();
}

MainWindow::~MainWindow()
{
}

