#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QList>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

// структура в которой мы будем зранить данные
struct Data {
public:
    QString key;
    double value;
};

//итнерфейс получения данных
class IDataStructure {
public:
    virtual QList<Data> getData(QString) = 0; //ф-я получения данных
    virtual ~IDataStructure() = default;
};

class SqlDataStructure: public IDataStructure { //получение данных от sqlite
public:
    QList<Data> getData(QString);
    ~SqlDataStructure() = default;
};

class JsonDataStructure: public IDataStructure { // получение данных от json
public:
    QList<Data> getData(QString);
    ~JsonDataStructure() = default;
};

#endif // DATASTRUCTURE_H
