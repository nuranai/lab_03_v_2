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
    //добавляем в combobox виды графиков
    comboboxChartType->addItem("Bar");
    comboboxChartType->addItem("Pie");

    // creating layouts
    wrapperLayout = new QHBoxLayout(this); //внешнее расположение
    fileExplorerLayout = new QVBoxLayout(); // расположение для обзора файлов
    chartLayout = new QVBoxLayout(); // расположение для графиков
    chartWidgetLayout = new QHBoxLayout(); // расположение для виджетов графика

    fileSplitter = new QSplitter();
    chartSplitter = new QSplitter();

    // добавление расположений

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

    // устанавливаем текущую папку для просмотра
    directoryPath = QDir::currentPath();
    pathLabel = new QLabel();
    //устанавливаем в метку текущую директорию
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

    //подключение слотов
    //подключение изменения директории
    connect(btnChangeDirectory, SIGNAL(clicked(bool)), this, SLOT(changeDirectory()));
    //подключение изменения выранного файла
    connect(
                tableFileView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection, const QItemSelection)),
                this, SLOT(fileSelection(const QItemSelection, const QItemSelection))
                );
    // подключение изменения типа графика
    connect(comboboxChartType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeChartType()));
    // подключение изменения цвета графика
    connect(chkbxBlackWhiteChart, SIGNAL(toggled(bool)), this, SLOT(colorSwap()));
    // подключение печать графика
    connect(btnPrintChart, SIGNAL(clicked()), this, SLOT(printChart()));
}

void MainWindow::changeDirectory() {
    //диалоговое окно
    QFileDialog dialogWindow(this);
    //вид только папок
    dialogWindow.setFileMode(QFileDialog::Directory);
    //если открылось
    if (dialogWindow.exec()) {
        //обновляем путь
        directoryPath = dialogWindow.selectedFiles().first();
        pathLabel->setText(directoryPath);
    }
    tableFileView->setRootIndex(fileModel->setRootPath(directoryPath));
}

void MainWindow::fileSelection(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);

    QModelIndexList indexes = selected.indexes();
    // если количество инедксов < 1 вызываем ошибку
    if (indexes.count() < 1) {
        exceptionCall("Selection Error", "No items has been Selected");
        return;
    }
    // получаем выбранный путь до файла
    filePath = fileModel->filePath(indexes.first());
    // если jsin формат
    if (filePath.endsWith(".json")) {
        iocContainer.RegisterInstance<IDataStructure, JsonDataStructure>();
    }
    else if (filePath.endsWith(".sqlite")) {
        iocContainer.RegisterInstance<IDataStructure, SqlDataStructure>();
    }
    else {
        // если формат не верен вызывваем ошибку
        exceptionCall("Wrong file format", "Please select .json or .sqlite file");
        // если график активен очищаем его
        if (isChartActive) {
            auto chart = iocContainer.GetObject<IChart>();
            chart->getChart()->setTitle("");
            chart->clearChart();
            isChartActive = false;
        }
        return;
    }
    // проверяем тип графика
    if (comboboxChartType->currentText() == "Pie") {
        iocContainer.RegisterInstance<IChart, PieChart>();
        isChartActive = true;
    }
    else if (comboboxChartType->currentText() == "Bar") {
        iocContainer.RegisterInstance<IChart, BarChart>();
        isChartActive = true;
    }
    // рисуем график
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::changeChartType() {
    if (comboboxChartType->currentText() == "Pie") {
        //используя внедрение зависимости создаем график типа pie
        iocContainer.RegisterInstance<IChart, PieChart>();
    }
    else if (comboboxChartType->currentText() == "Bar") {
        //используя внедрение зависимости создаем график типа bar
        iocContainer.RegisterInstance<IChart, BarChart>();
    }
    // если график активен перерисовываем его
    if (isChartActive) {
        drawChart();
    }
}

void MainWindow::colorSwap() {
    //если график активен перерисовываем его
    if (isChartActive) drawChart();
}

void MainWindow::printChart() {
    //печатаем график только когда он активен
    if(isChartActive){
        QFileDialog *fileDialog = new QFileDialog(this);
        // заголовок файла
        fileDialog-> setWindowTitle (tr ("Сохранить в ..."));
        //путь к файлу определяем по умолчанию
        fileDialog->setDirectory(".");
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        // Установить режим просмотра
        fileDialog->setViewMode(QFileDialog::Detail);
        QStringList fileNames;
        if(fileDialog->exec())
        {
            fileNames = fileDialog->selectedFiles();
        }

        QPdfWriter pdfWriter(fileNames.first() + ".pdf");//название

        pdfWriter.setCreator("User");

        pdfWriter.setPageSize(QPageSize::A4);//размер страницы А4

        pdfWriter.setResolution (300);//разрешение бумаги на 300, чтобы пиксель был 3508 * 2479
        // Добавляем контент с помощью QPainter
        QPainter painter(&pdfWriter);

        //Отрисовка
        chartView->render(&painter);
        painter.end();
    }
    else {
        //вызываем ошибку что график не активен
        exceptionCall("Pdf error", "No chart to print");
    }
}

void MainWindow::drawChart() {
    //используя ioc обращение к интерфейсам- базовым классам работы с графиками и работы с данными
    auto chart = iocContainer.GetObject<IChart>();
    auto dataStructure = iocContainer.GetObject<IDataStructure>();
    // получаем данные
    QList<Data> items = dataStructure->getData(filePath);
    // рисуем график на основе данных и передаем значения цвета нашего графика
    chart->recreateChart(items, chkbxBlackWhiteChart->isChecked());
    //получаем представление графика
    chartView->setChart(chart->getChart());
}

void MainWindow::exceptionCall(QString title, QString message) {
    // создаем messagebox
    QMessageBox *messageBox = new QMessageBox();
    //устанавливаем заголовок
    messageBox->setWindowTitle(title);
    //устанавливаем сообщение
    messageBox->setText(message);
    //открываем
    messageBox->exec();
}

MainWindow::~MainWindow()
{
}
