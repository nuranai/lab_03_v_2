#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <QList>
#include <QFile>


class IDataStructure {
public:
    virtual void getData(QString);
};

class SqlDataStructure: IDataStructure {
public:
    void getData(QString);
};

class JsonDataStructure: IDataStructure {
public:
    void getData(QString);
};

#endif // DATASTRUCTURE_H
