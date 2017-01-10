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

#ifndef BIOGUI_EXECUTETHREAD_H
#define BIOGUI_EXECUTETHREAD_H

#include <QThread>

#include <iostream>


class ExecuteThread : public QThread
{
    Q_OBJECT

public:

    ExecuteThread() : QThread()
    {
    }

    virtual ~ExecuteThread()
    {

        std::cerr << QThread::currentThreadId() << " destroyed" << std::endl;

    }


    virtual QByteArray readAllStandardOutput() = 0;
    virtual QByteArray readAllStandardError() = 0;

public slots:

    virtual void startExecution() //QPrivateSignal* pSignal
    {
        this->execute();
    }

    signals:

    void threadRunned();
    void executionFinished();

    void readyReadStandardOutput();
    void readyReadStandardError();


protected:

    virtual void run()
    {
        this->connect(this, &ExecuteThread::threadRunned, this, &ExecuteThread::startExecution, Qt::DirectConnection);

        emit threadRunned();

        this->exec();
    }

    virtual void execute() = 0;

    //ExtendedThreadBuffer* m_pSTDBuffer;
    //ExtendedThreadBuffer* m_pERRBuffer;


};


#endif //BIOGUI_EXECUTETHREAD_H
