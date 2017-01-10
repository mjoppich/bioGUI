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

#ifndef UTILS_MATHS_H
#define UTILS_MATHS_H

#include <vector>
#include <inttypes.h>
#include <algorithm>

template <typename T>
class Maths {
public:


    /**
     * \brief calculates quantile of a vector of numbers
     * \param pVec pointer to vector of numbers
     * \param fPerc specifies the quantile. Median is 0.5 quantile
     * \return floating point median value of vector
     */
    static float getQuantile(std::vector<T>* pVec, float fPerc) {

        if ((pVec == NULL) || (pVec->size() == 0))
            return -2.0f;

        float fPercPos = (float) pVec->size() * fPerc;
        uint32_t iPercPos = uint32_t(fPercPos);

        float fQuantile = 0.0f;

        if ( fPercPos ==  iPercPos)
        {
            fQuantile = pVec->at( iPercPos );
        } else {

            uint32_t iLeftPos = std::min((size_t) iPercPos, pVec->size()-1);
            uint32_t iRightPos = std::min((size_t) iPercPos+1, pVec->size()-1);

            uint32_t iHalfSum = pVec->at( iLeftPos ) + pVec->at( iRightPos );

            fQuantile = (float)iHalfSum / 2.0f;

        }

        return fQuantile;
    }

    /**
     * \brief calculates N50 value of a vector of numbers (see wikipedia to understand the meaning of N50)
     * \param pVec pointer to vector of numbers
     * \return floating point N50 value of vector
     */
    static float getN50(std::vector<T>* pVec)
    {
        float fHalf = Maths<T>::getSum(pVec);

        return getN50(pVec, fHalf);
    }

    /**
     * \brief calculates N50 value of a vector of numbers (see wikipedia to understand the meaning of N50)
     * \param pVec pointer to vector of numbers
     * \param fHalf sum of pVec / 2.0
     * \return floating point N50 value of vector
     */
    static float getN50(std::vector<T>* pVec, float fHalf)
    {
        if ((pVec == NULL) || (pVec->size() == 0))
            return -2.0f;

        uint32_t iTmpSum = 0;
        for (uint32_t i = pVec->size(); i >= 1; --i)
        {
            iTmpSum += pVec->at(i-1);

            if (iTmpSum > fHalf)
            {
                float fN50Length = pVec->at(i-1);
                return fN50Length;
            }

        }
    }

    /**
     * \brief calculates sum of a vector of numbers
     * \param pVec pointer to vector of numbers
     * \return floating point sum value of vector
     */
    static T getSum(std::vector<T>* pVec)
    {
        if ((pVec == NULL) || (pVec->size() == 0))
            return 0;

        T iTmpSum = 0;
        for (uint32_t i = pVec->size(); i >= 1; --i)
        {
            iTmpSum += pVec->at(i-1);

        }

        return iTmpSum;
    }


    /**
     * \brief calculates mean of a vector of numbers
     * \param pVec pointer to vector of numbers
     * \return floating point mean value of vector
     */
    static float getMean(std::vector<T>* pVec)
    {
        if ((pVec == NULL) || (pVec->size() == 0))
            return 0.0f;

        T iSum = Maths::getSum(pVec);

        float fMean = (float) iSum / (float) pVec->size();
        return fMean;
    }



    /**
     *
     * \brief returns the integer factor needed to transform a floating point value into an integer with minimal loss
     *
     * calculates: 0.1 => 1 is closest integer by factor 10 -> return 10
     *
     * \param fValue the number to get the factor for
     * \param iMaxIterations max number if iterations. If machine precision is reached earlier, the minimum is chosen
     * \return Factor to transform a decimal number into integer
     *
     */
    static int getIntegerFactor(T fValue, int iMaxIterations = 20)
    {

        int iIterations = 0;

        // iTransform is integered version of fValue
        int iTransform = fValue / 1;
        int iFactor = 1;
        while ( iIterations++ < iMaxIterations )
        {

            if (std::abs( fValue * (float) iFactor - iTransform) <= std::numeric_limits<T>::epsilon() )
            {
                break;
            }

            iFactor = iFactor * 10;
            iTransform = fValue * (T) iFactor;


        }

        return iFactor;


    }
};


#endif //UTILS_MATHS_H
