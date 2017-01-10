/*
 *  bioGUI - a new universal GUI for bioinformatic applications
 *  Copyright (C) 2016  Markus Joppich
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
