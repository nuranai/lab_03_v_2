#include "chart.h"

void BarChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("asd");
    QBarSeries *series = new QBarSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        QBarSet* set = new QBarSet(list.at(i).key);
        * set << list.at(i).value;
        if (isBWEnabled)
        {
            //меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
            int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            // присваиваем элементам диаграммы эти цветовые данные
            set->setBrush(QColor(bw, bw, bw));
        }
        series->append(set);
    }
    chart->removeAllSeries();
    chart->addSeries(series);
    chart->createDefaultAxes();
}

void PieChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    Data data;
    QChart* chart = getChart();
    chart->setTitle("bamp");
    QPieSeries * series = new QPieSeries(chart);
    int length = list.count();
    for (int i = 0; i < length; i++) {
        QPieSlice* slice = series->append(list.at(i).key, list.at(i).value);
        if (isBWEnabled)
        {
            //меняем цвет на чб, используя рандомную раскладку оттенков серого от белого к черному
            int bw = (rand() % 255 + rand() % 255 + rand() % 255) / 3;
            // присваиваем элементам диаграммы эти цветовые данные
            slice->setBrush(QColor(bw, bw, bw));
        }
    }
    chart->removeAllSeries();
    chart->addSeries(series);
}
