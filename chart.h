#ifndef CHART_H
#define CHART_H
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <datastructure.h>

class IChart {
    QChart* chart = new QChart();
public:
    QChart* getChart() {return chart;}
    virtual void recreateChart(QList<Data>) = 0;
    virtual ~IChart() = default;
};

class BarChart: public IChart {
public:
    void recreateChart(QList<Data>);
    ~BarChart() = default;
};

class PieChart: public IChart {
public:
    void recreateChart(QList<Data>);
    ~PieChart() = default;
};

#endif // CHART_H
