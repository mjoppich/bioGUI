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

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include "bioGUIapp.h"

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{

    std::ofstream out("log.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::streambuf *cerrbuf = std::cerr.rdbuf();

    std::cout.rdbuf(out.rdbuf());
    std::cerr.rdbuf(out.rdbuf());

    freopen("output.txt","w",stdout);


    bioGUIapp* pApp = new bioGUIapp(argc, argv);
    pApp->exec();
    delete pApp;

    return 0;
}
