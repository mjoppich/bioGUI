//
// Created by joppich on 12/6/16.
//

#ifndef BIOGUI_NULLABLE_H
#define BIOGUI_NULLABLE_H


template <class T>
class Validable
{

public:

    Validable(T oValue, bool bValid = true)
    {
        m_oValue = oValue;
        m_bIsValid = bValid;
    }

    T& ref()
    {
        return m_oValue;
    }

    T value()
    {
        return m_oValue;
    }

    T* ptr()
    {
        return &m_oValue;
    }

    bool valid()
    {
        return m_bIsValid;
    }

    void setValue(T oValue, bool bValid = true)
    {
        m_oValue = oValue;
        m_bIsValid = bValid;
    }


protected:

    bool m_bIsValid;
    T m_oValue;

};

#endif //BIOGUI_NULLABLE_H
