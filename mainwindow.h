#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QTableView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QtCharts/QChartView>
#include <QMessageBox>
#include <QPdfWriter>
#include "chart.h"


class MainWindow : public QWidget
{
    Q_OBJECT

private slots:
    void changeDirectory(); // слот смены директории
    //слот изменения выбранного файла в таблице
    void fileSelection(const QItemSelection &selected, const QItemSelection &deselected);
    //слот изменения типа графика
    void changeChartType();
    //слот печати диаграммы
    void printChart();
    //слот смены цвета диаграммы
    void colorSwap();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // строка хранящая директорию
    QString directoryPath;
    // строка хранящая путь до файла
    QString filePath;

    QFileSystemModel* fileModel;
    QTableView* tableFileView;

    QChartView* chartView;

    QLabel* pathLabel; //label that shows current path

    QPushButton* btnPrintChart; //button to [rint charts
    QPushButton* btnChangeDirectory; //button to change directory in table

    QCheckBox* chkbxBlackWhiteChart; //checkbox to change charts colors

    QComboBox* comboboxChartType; // combobox to change chart type

    QHBoxLayout* wrapperLayout; // outer layout
    QVBoxLayout* fileExplorerLayout; // layout for file explorer and it's widgets
    QVBoxLayout* chartLayout; // layout for chart and it's widgets
    QHBoxLayout* chartWidgetLayout; // layout of the chart widgets

    QSplitter* fileSplitter;
    QSplitter* chartSplitter;

    bool isChartActive = false; //переменная хранящая активность графика

    void exceptionCall(QString title, QString message); // вызов messagebox
    void drawChart(); // рисовка графика
};
#endif // MAINWINDOW_H
