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
#include "chart.h"


class MainWindow : public QWidget
{
    Q_OBJECT

private slots:
    void changeDirectory();
    void fileSelection(const QItemSelection &selected, const QItemSelection &deselected);
    void changeChartType();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QString directoryPath;
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

    bool isChartActive = false;

    void exceptionCall(QString title, QString message);
    void drawChart();
};
#endif // MAINWINDOW_H
