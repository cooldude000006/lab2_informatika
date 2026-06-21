#pragma once

#include <cstddef>
#include "exceptions.h"
#include "ienumerable.h"
#include "icollection.h"
#include "option.h"
#include <ostream>
namespace lab2
{
    template <class T>
    class Sequence :
    public IEnumerable<T>,
    public ICollection<T>
    {
    public:
        virtual ~Sequence() = default;

        std::size_t GetCount() const override
        {
            return static_cast<std::size_t>(
                GetLength()
            );
        }

        T Get(std::size_t index) const override
        {
            if (index >= GetCount())
            {
                throw IndexOutOfRangeException(
                    static_cast<int>(index),
                    GetLength(),
                    "ICollection::Get"
                );
            }

            return Get(static_cast<int>(index));
        }

        //try-семантика(получение элемента по индексу)
        Option<T> TryGet(int index) const
        {
            if (index < 0 || index >= GetLength()) return Option<T>::None();
            return Option<T>::Some(Get(index));
        }

        Option<T> TryFirst() const
        {
            if (GetLength() == 0) return Option<T>::None();
            return Option<T>::Some(GetFirst());
        }

        Option<T> TryLast() const
        {
            if (GetLength() == 0) return Option<T>::None();
            return Option<T>::Some(GetLast());
        }

        // итератор
        IEnumerator<T>* GetEnumerator() const override = 0;

        //декомпозиция
        virtual T GetFirst() const = 0; //получить первый элемент

        virtual T GetLast() const = 0;//получить последний элемент

        virtual T Get(int index) const = 0;//получить элемент по индексу

        virtual  Sequence<T>*  GetSubsequence(int startIndex, int endIndex) const = 0;//получить подпоследовательность

        virtual int GetLength() const = 0;//получить количество элементов

        //операции
        virtual Sequence<T>* Append(const T& item) = 0;//добавить эл в конец

        virtual Sequence<T>* Prepend(const T& item) = 0;//добавить эл в начало

        virtual Sequence<T>* InsertAt(const T& item, int index) = 0;//вставить эл ао индексу

        virtual Sequence<T>* Concat(Sequence<T>* list) const = 0;//сцепить с другой последовательностью

        //map-reduce
        template<typename Func>
        Sequence<T>* Map(Func f) const
        {
            Sequence<T>* result = CreateEmpty();

            try
            {
                for (int i = 0; i < GetLength(); ++i)
                {
                    Sequence<T>* next =
                        result->Append(f(Get(i)));

                    if (next != result)
                    {
                        delete result;
                        result = next;
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }
            return result;
        }

        template<typename Func>
        Sequence<T>* MapWithIndex(Func function) const
        {
            Sequence<T>* result = CreateEmpty();

            try
            {
                for (int index = 0; index < GetLength(); ++index)
                {
                    T newValue =
                        function(Get(index), index);

                    Sequence<T>* next =
                        result->Append(newValue);

                    if (next != result)
                    {
                        delete result;
                        result = next;
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }

            return result;
        }

        template<typename Predicate>
        Sequence<T>* Where(Predicate pred) const
        {
            Sequence<T>* result = CreateEmpty();

            try
            {
                for (int i = 0; i < GetLength(); ++i)
                {
                    T item = Get(i);

                    if (pred(item))
                    {
                        Sequence<T>* next =
                            result->Append(item);

                        if (next != result)
                        {
                            delete result;
                            result = next;
                        }
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }

            return result;
        }

        template <typename Func, typename TAcc> //свернкть последовательность в одно число
        TAcc Reduce(Func f, TAcc initial) const
        {
            TAcc acc = initial;
            for (int i=0; i<GetLength(); ++i)
            {
                acc = f(acc, Get(i));
            }
            return acc;
        }

        //
        T operator[](int index) const //оператор [] доступа по индексу(перегрузка)
        {
            return Get(index);
        }
        //

    protected:
        virtual Sequence<T>* CreateEmpty() const = 0; //вспомогательный метод создания пустой последовательности того же типа
    };
    //перегрузка operator <<
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Sequence<T>& seq)
    {
        os << "[";
        for (int i = 0; i < seq.GetLength(); ++i)
        {
            os << seq.Get(i);
            if (i < seq.GetLength() - 1) os << ", ";
        }
        return os << "]";
    }
}






























