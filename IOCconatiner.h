#ifndef IOCCONATINER_H
#define IOCCONATINER_H

#include <iostream>
#include <functional>
#include <cstdlib>
#include <memory>
#include <map>

using namespace std;


class IOCContainer

{

    static int s_typeId;// определяет идентификатор следующего типа, который будет выделен,
    //и экземпляр статической локальной переменной для каждого типа, к которому можно обратиться вызвав метод GetTypeID.

public:

    template<typename T>
    static int GetTypeID()
    {

        static int typeId = s_typeId++;
        return typeId;
    }

    /*
     Получение экземпляров объекта;
     Теперь, когда  есть идентификатор типа, мы должны иметь возможность хранить какой-то фабричный объект,
     чтобы показать тот факт, что мы не знаем, как создать объект.(так как мы не знаем как создать объект)
     Хранить все фабрики в одной коллекции, для этого выбираем абстрактный базовый класс,
     из которого будут производиться фабрики, и реализацию, которая захватывает функтор для последующего вызова.
     Можно использовать std :: map для хранения фабрик,та же можно рассмотреть  другие варианты для эффективности.
    */
    class FactoryBase

    {

    public:

        virtual ~FactoryBase() =default;

    };

    //todo: consider sorted vector

    map<int, shared_ptr<FactoryBase>> factories;//ХРАНИМ ФАБРИКИ

    template<typename T>

    class CFactory : public FactoryBase

    {
        //std::function
        std::function<std::shared_ptr<T>()> functor;
    public:
        ~CFactory() {}

        CFactory(std::function<std::shared_ptr<T>()> functor)

            :functor(functor)

        {

        }

        std::shared_ptr<T> GetObject()
        {
            return functor();
        }

    };

    template<typename T>

    std::shared_ptr<T> GetObject()

    {

        auto typeId = GetTypeID<T>();
        auto factoryBase = factories[typeId];
        auto factory = std::static_pointer_cast<CFactory<T>>(factoryBase);
        return factory->GetObject();
    }

    /*
    Регистрация экземпляров
    */

    //Базовая реализация - регистрация функтора

    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)> functor)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return functor(GetObject<TS>()...); });
    }

    //Регистрация экземпляра объекта
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> t)
    {
        factories[GetTypeID<TInterface>()] = std::make_shared<CFactory<TInterface>>([=] {return t; });
    }

    //Работаем с указателем на функцию
    template<typename TInterface, typename ...TS>
    void RegisterFunctor(std::shared_ptr<TInterface>(*functor)(std::shared_ptr<TS> ...ts))
    {
        RegisterFunctor(std::function<std::shared_ptr<TInterface>(std::shared_ptr<TS> ...ts)>(functor));
    }


    static IOCContainer& IOCInstance()//для получения единственного экземпляра объекта
        {
            static IOCContainer ioc_;
            return ioc_;
        }

    //Фабрика, которая будет вызывать необходимый конструктор для каждого экземпляра
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterFactory()
    {
        RegisterFunctor(
            std::function<std::shared_ptr<TInterface>(std::shared_ptr<TArguments> ...ts)>(
                [](std::shared_ptr<TArguments>...arguments) -> std::shared_ptr<TInterface>
        {
            return std::make_shared<TConcrete>(std::forward<std::shared_ptr<TArguments>>(arguments)...);
        }));

    }





    //Фабрика, которая будет возвращать один экземпляр  объекта  для каждого вызова
    template<typename TInterface, typename TConcrete, typename ...TArguments>
    void RegisterInstance()
    {
        RegisterInstance<TInterface>(std::make_shared<TConcrete>(GetObject<TArguments>()...));
    }


};

IOCContainer iocContainer;

int IOCContainer::s_typeId = 1;

#endif // IOCCONTAINER_H
