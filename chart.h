#ifndef CHART_H
#define CHART_H
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <datastructure.h>

class IChart { // общий интерфейс
    QChart* chart = new QChart(); //диаграмма
public:
    QChart* getChart() {return chart;} //геттер
    virtual void recreateChart(QList<Data>, bool) = 0; //построение диаграммы
    virtual ~IChart() = default;
    void clearChart() {chart->removeAllSeries();} //очистка
};

class BarChart: public IChart { // диаграмма типа bar
public:
    void recreateChart(QList<Data>, bool);
    ~BarChart() = default;
};

class PieChart: public IChart { // диаграмма типа pie
public:
    void recreateChart(QList<Data>, bool);
    ~PieChart() = default;
};

#endif // CHART_H
