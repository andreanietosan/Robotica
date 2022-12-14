/*
 *    Copyright (C) 2022 by YOUR NAME HERE
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "specificworker.h"

/**
* \brief Default constructor
*/



SpecificWorker::SpecificWorker(TuplePrx tprx, bool startup_check) : GenericWorker(tprx)
{
    this->startup_check_flag = startup_check;
    // Uncomment if there's too many debug messages
    // but it removes the possibility to see the messages
    // shown in the console with qDebug()
//	QLoggingCategory::setFilterRules("*.debug=false\n");
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{
    std::cout << "Destroying SpecificWorker" << std::endl;
}


bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
//    id_giraff=std::stoi(params.at("id_giraff").value);
    return true;
}


void SpecificWorker::initialize(int period)
{
    std::cout << "Initialize worker" << std::endl;
    this->Period = period;
    if(this->startup_check_flag)
    {
        this->startup_check();
    }
    else
    {
        timer.start(Period);
    }

}

void SpecificWorker::compute()
{
    //robot control
    RoboCompLaserMulti::TLaserData ldata;
    try
    { ldata = lasermulti_proxy->getLaserData(0);}
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; return ;}


    std::tuple<float, float> tuplaAux;
    switch(state)
    {
        case State::FORWARD:
            tuplaAux=fFORWARD(ldata);
            break;

        case State::TURN:
            tuplaAux=fTURN(ldata);
            break;

        case State::FOLLOW_WALL:
            tuplaAux=fFOLLOW_WALL(ldata);
            break;

        case State::SPIRAL:
            tuplaAux=fSPIRAL(ldata);
            break;
    }

    //robot actua
    qInfo()<< "addv: "<< get<0>(tuplaAux) << " rot:" << get<1>(tuplaAux);
    try
    { differentialrobotmulti_proxy->setSpeedBase(0, get<0>(tuplaAux),get<1>(tuplaAux));}
    catch (const Ice::Exception &e) {std::cout << e.what() << std::endl; }
}

///////////////////////////////////////////////////////////////////////////////////////

float SpecificWorker::realizarMedia(RoboCompLaserMulti::TLaserData &copy)
{
    float suma=0, media;
    for (long unsigned int i=0; i < copy.size(); i++)
    {
        suma+=copy.at(i).dist;
    }

    media=suma/copy.size();

    return media;
}


tuple<float, float> SpecificWorker::fFORWARD(RoboCompLaserMulti::TLaserData &ldata)
{
    const int part = 3;
    RoboCompLaserMulti::TLaserData copyFrente(ldata.begin()+(ldata.size()/part),ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyFrente,{},&RoboCompLaserMulti::TData::dist);

    RoboCompLaserMulti::TLaserData copyAll(ldata.begin(), ldata.end());
    std::ranges::sort(copyAll, {},&RoboCompLaserMulti::TData::dist);

    qInfo() <<"FORWARD:"<< " distancia:" <<copyFrente.front().dist;

    tuple<float, float> tuplaAux;

    if(copyFrente.front().dist < consts.REFERENCE_DISTANCE)
    {
        primera_vez_turn=true;
        modo=rand()%2;
        state = State::TURN;
        tuplaAux = make_tuple(0, 0);
    }
    else if (copyAll.front().dist >= consts.MAX_SPIRAL)
    {
        state = State::SPIRAL;
        tuplaAux = make_tuple(0, 0);
    }
    else
        tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FORWARD, 0);

    return tuplaAux;
}


bool SpecificWorker::detectarPared(RoboCompLaserMulti::TLaserData &ldata){
    bool pared=false;
    const int part = 3;

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyDer(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyDer, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyIzq(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyIzq, {},&RoboCompLaserMulti::TData::dist);

    float mediaIzq = realizarMedia(copyIzq);
    float mediaDer = realizarMedia(copyDer);

    if(mediaIzq < mediaDer){
        if(mediaIzq < 2400){
            pared=true;
        }
    }else{
        if(mediaDer < 2400){
            pared=true;
        }
    }
    return pared;
}

tuple<float, float> SpecificWorker::fTURN(RoboCompLaserMulti::TLaserData &ldata)
{
    static bool giro_derecha;

    //Laser parte central
    const int part = 3;
    RoboCompLaserMulti::TLaserData copyFrente(ldata.begin()+(ldata.size()/part), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyFrente, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte derecha
    RoboCompLaserMulti::TLaserData copyDer(ldata.begin(), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyDer, {},&RoboCompLaserMulti::TData::dist);

    //Laser parte izquierda
    RoboCompLaserMulti::TLaserData copyIzq(ldata.begin()+(ldata.size()/part), ldata.end());
    std::ranges::sort(copyIzq, {},&RoboCompLaserMulti::TData::dist);

    qInfo() << "TURN:" << " distancia:" << copyFrente.front().dist;
    qInfo() << "TURN:" <<  " mediaIzq:" << realizarMedia(copyIzq) << " mediaDer:"<< realizarMedia(copyDer);

    tuple<float, float> tuplaAux;

    if(copyFrente.front().dist >= consts.REFERENCE_DISTANCE)
    {
        //Dependiendo del movimiento random, unas veces irá recto, otras hará seguir pared.
        qInfo()<<"SE ELIGE EL MODO:" << modo;
        //Si sale un 1 y esta pegado en la pared realiza el seguir pared, sino va recto
        if(modo==1 && detectarPared(ldata))
            state = State::FOLLOW_WALL;
        else
            state = State::FORWARD;

        tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, 0);
    }
    else
    {
        if (primera_vez_turn)
        {
            if (realizarMedia(copyIzq) > realizarMedia(copyDer))
                giro_derecha = false;
            else
                giro_derecha = true;

            primera_vez_turn = false;
        }

        if (giro_derecha)
            tuplaAux = make_tuple(0, consts.ROT_TURN);
        else
            tuplaAux = make_tuple(0, -consts.ROT_TURN);
    }
    return tuplaAux;
}


tuple<float, float> SpecificWorker::fFOLLOW_WALL(RoboCompLaserMulti::TLaserData &ldata)
{
    static std::chrono::time_point<std::chrono::system_clock> start;
    if (primera_vez_fw)
    {
        start = std::chrono::system_clock::now();
        primera_vez_fw=false;
    }

    //laser parte central
    const int part = 3;
    RoboCompLaserMulti::TLaserData copyFrente(ldata.begin()+(ldata.size()/part), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyFrente, {},&RoboCompLaserMulti::TData::dist);

    //laser parte izquierda
    RoboCompLaserMulti::TLaserData copyIzq(ldata.begin()+(2*(ldata.size()/part)), ldata.end());
    std::ranges::sort(copyIzq, {},&RoboCompLaserMulti::TData::dist);

    //laser parte derecha
    RoboCompLaserMulti::TLaserData copyDer(ldata.begin(), ldata.end()-(2*(ldata.size()/part)));
    std::ranges::sort(copyDer, {},&RoboCompLaserMulti::TData::dist);

    tuple<float, float> tuplaAux;

    float mediaIzq = realizarMedia(copyIzq);
    float mediaDer = realizarMedia(copyDer);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<float,std::milli> duration = end - start;

    qInfo()<<"FOLLOW_WALL"<<"Tiempo de ejecucion:"<<duration.count();

    if(copyFrente.front().dist < consts.REFERENCE_DISTANCE-200 ||  duration.count() >  consts.TIEMPO_DIFERENCIA)
    {
        if(duration.count() > consts.TIEMPO_DIFERENCIA)
        {
            qInfo()<<"HA CUMPLIDO LOS 10s SIGUIENDO LA PARED";
            modo = rand()%2;
            primera_vez_fw = true;
            tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, 0);
        }
        else
        {
            modo = 1;
            tuplaAux = make_tuple(0, 0);
        }
        primera_vez_turn=true;
        state = State::TURN;

    }
    else
    {
        qInfo() << "FOLLOW_WALL:" << " distancia:" << copyFrente.front().dist;

        qInfo() << "FOLLOW_WALL:" << " mediaIzq:" << mediaIzq << " mediaDer:" << mediaDer;

        if (mediaIzq < mediaDer) //pared a la izq
        {
            if (mediaIzq > consts.REFERENCE_DISTANCE + consts.DELTA) {
                qInfo() << "FOLLOW_WALL:" << "corrige trayectoria:";
                tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, -0.25);
            } else {
                if (mediaIzq < consts.REFERENCE_DISTANCE - consts.DELTA) {
                    qInfo() << "FOLLOW_WALL:" << "corrige trayectoria:";
                    tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, +0.25);
                } else {
                    tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, 0);
                }
            }
        } else //pared a la der
        {
            if (mediaDer > consts.REFERENCE_DISTANCE + consts.DELTA) {
                qInfo() << "FOLLOW_WALL:" << "corrige trayectoria:";
                tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, +0.25);
            } else {
                if (mediaDer < consts.REFERENCE_DISTANCE - consts.DELTA) {
                    qInfo() << "FOLLOW_WALL:" << "corrige trayectoria:";
                    tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, -0.25);
                } else {
                    tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FW, 0);
                }
            }
        }
    }

    return tuplaAux;
}


tuple<float, float> SpecificWorker::fSPIRAL(RoboCompLaserMulti::TLaserData &ldata)
{
    static float addvSpiral=1;
    static float rotSpiral=consts.MAX_ROT_SPPED;

    const int part = 3;
    RoboCompLaserMulti::TLaserData copyFrente(ldata.begin()+(ldata.size()/part), ldata.end()-(ldata.size()/part));
    std::ranges::sort(copyFrente, {},&RoboCompLaserMulti::TData::dist);

    qInfo() <<"SPIRAL:"<< " distancia:" <<copyFrente.front().dist;

    tuple<float, float> tuplaAux = make_tuple(addvSpiral, rotSpiral);

    if(copyFrente.front().dist < consts.REFERENCE_DISTANCE)
    {
        primera_vez_turn=true;
        modo=rand()%2;
        state = State::TURN;
        tuplaAux = make_tuple(0, 0);
        addvSpiral=1;
        rotSpiral=consts.MAX_ROT_SPPED;
    }
    else {
        if(addvSpiral <= consts.MAX_ADV_SPEED_SPIRAL && rotSpiral >= 0)
        {
            sleep(1);
            addvSpiral+=35;
            rotSpiral-=0.012;
        }else{
            state = State::FORWARD;
            tuplaAux = make_tuple(consts.MAX_ADV_SPEED_FORWARD, 0);
            addvSpiral=1;
            rotSpiral=consts.MAX_ROT_SPPED;
        }
    }

    return tuplaAux;
}


int SpecificWorker::startup_check()
{
    std::cout << "Startup check" << std::endl;
    QTimer::singleShot(200, qApp, SLOT(quit()));
    return 0;
}


/**************************************/
// From the RoboCompDifferentialRobot you can call this methods:
// this->differentialrobot_proxy->correctOdometer(...)
// this->differentialrobot_proxy->getBasePose(...)
// this->differentialrobot_proxy->getBaseState(...)
// this->differentialrobot_proxy->resetOdometer(...)
// this->differentialrobot_proxy->setOdometer(...)
// this->differentialrobot_proxy->setOdometerPose(...)
// this->differentialrobot_proxy->setSpeedBase(...)
// this->differentialrobot_proxy->stopBase(...)

/**************************************/
// From the RoboCompDifferentialRobot you can use this types:
// RoboCompDifferentialRobot::TMechParams

/**************************************/
// From the RoboCompLaser you can call this methods:
// this->laser_proxy->getLaserAndBStateData(...)
// this->laser_proxy->getLaserConfData(...)
// this->laser_proxy->getLaserData(...)

/**************************************/
// From the RoboCompLaser you can use this types:
// RoboCompLaser::LaserConfData
// RoboCompLaser::TData
