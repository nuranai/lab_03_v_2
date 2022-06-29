#include "chart.h"

void BarChart::recreateChart(QList<Data> list) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("asd");
    QBarSeries *series = new QBarSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        QBarSet* set = new QBarSet(list.at(i).key);
        * set << list.at(i).value;
        series->append(set);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

void PieChart::recreateChart(QList<Data> list) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("bamp");
    QPieSeries * series = new QPieSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        series->append(list.at(i).key, list.at(i).value);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
}
