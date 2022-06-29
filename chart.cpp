#include "chart.h"

void BarChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    // полученные данные из списка
    Data data;
    // диаграмма
    QChart* chart = getChart();
    // заголовок
    chart->setTitle("BarChart");
    QBarSeries *series = new QBarSeries(chart);
    // количество данных
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
    // очищаем всю серии chart
    chart->removeAllSeries();
    // задаем новые
    chart->addSeries(series);
    chart->createDefaultAxes();
}

void PieChart::recreateChart(QList<Data> list, bool isBWEnabled) {
    // полученные данные из списка
    Data data;
    // диаграмма
    QChart* chart = getChart();
    // заголовок
    chart->setTitle("PieChart");
    QPieSeries * series = new QPieSeries(chart);
    // количество данных
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
    // очищаем всю серии chart
    chart->removeAllSeries();
    // задаем новые
    chart->addSeries(series);
}
