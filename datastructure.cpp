#include "datastructure.h"

QList<Data> SqlDataStructure::getData(QString filePath) {
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE"); //создаем соединение по умолчанию с драйвером "QSQLITE"
    // устанавливаем связь
    dbase.setDatabaseName(filePath);
    // результат который мы будем хранить
    QList<Data> result;
    // если открылась наща база данных
    if (dbase.open()) {
        // делаем запрос
        QSqlQuery query ("SELECT * FROM " + dbase.tables().takeFirst());
        int i = 0; // счетчик данных
        while (query.next() && i < 10) { // пока не кончатся данные с запроса или не получим 10 данных
            // увеличиваем счетчик
            i++;
            // задаем стркутуру
            Data temp{query.value(0).toString(), query.value(1).toDouble()};
            // отправляем в список
            result.push_back(temp);
        }
    }
    else {
        qDebug() << "sqlite was not opened";
    }
    return result;
}

QList<Data> JsonDataStructure::getData(QString filePath) {
    QString val;
    QFile file;
    file.setFileName(filePath);
    QList<Data> result;
    // открываем файл для чтения и если открылся
    if  (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        val = file.readAll();
        file.close();
        // открываем как json документ
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        // открываем как объект
        QJsonObject jsonObject = doc.object();
        // итерация по ключам
        QStringList keys (jsonObject.keys());
        int i = 0;
        QListIterator<QString> iterator(keys);
        while (iterator.hasNext() && i < 10) { //цикл по всем ключам или пока не достигнем лимита в 10 значений
            QString key = iterator.next();
            double value = jsonObject.value(key).toDouble();
            Data temp{key, value};
            result.push_back(temp);
            i++;
        }
    }
    else {
        qDebug() << "Json was not opened";
    }
    return result;
}
