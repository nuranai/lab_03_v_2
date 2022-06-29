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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QString directoryPath;

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
};
#endif // MAINWINDOW_H
