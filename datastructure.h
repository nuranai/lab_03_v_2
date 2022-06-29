#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QList>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>

struct Data {
public:
    QString key;
    double value;
};

class IDataStructure {
public:
    virtual QList<Data> getData(QString) = 0;
    virtual ~IDataStructure() = default;
};

class SqlDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~SqlDataStructure() = default;
};

class JsonDataStructure: public IDataStructure {
public:
    QList<Data> getData(QString);
    ~JsonDataStructure() = default;
};

#endif // DATASTRUCTURE_H
