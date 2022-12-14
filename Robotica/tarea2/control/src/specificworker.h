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
 *    along with RoboComp.  If not, see http://www.gnu.org/licenses/.
 */

/**
    \brief
    @author authorname
*/


#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>
#include <innermodel/innermodel.h>
#include <ranges>


class SpecificWorker : public GenericWorker
{
    Q_OBJECT
public:
    SpecificWorker(TuplePrx tprx, bool startup_check);
    ~SpecificWorker();
    bool setParams(RoboCompCommonBehavior::ParameterList params);

    float realizarMedia(RoboCompLaserMulti::TLaserData &copy);
    tuple<float, float> fFORWARD(RoboCompLaserMulti::TLaserData &ldata);
    bool detectarPared(RoboCompLaserMulti::TLaserData &ldata);
    tuple<float, float> fTURN(RoboCompLaserMulti::TLaserData &ldata);
    tuple<float, float> fFOLLOW_WALL(RoboCompLaserMulti::TLaserData &ldata);
    tuple<float, float> fSPIRAL(RoboCompLaserMulti::TLaserData &ldata);


public slots:
    void compute();
    int startup_check();
    void initialize(int period);

private:
    std::shared_ptr < InnerModel > innerModel;
    bool startup_check_flag;

    enum class State {FORWARD, TURN, FOLLOW_WALL, SPIRAL};
    State state = State::FORWARD;

    struct Constants
    {
        const float MAX_ADV_SPEED_FORWARD = 1500;
        const float ROT_TURN=0.55;
        const float MAX_ADV_SPEED_FW = 900;
        const float MAX_ADV_SPEED_SPIRAL = 1500;
        const float MAX_ROT_SPPED = 1.2;
        const float REFERENCE_DISTANCE = 1250;
        const float DELTA = 50;
        const float MAX_SPIRAL = 2000;
        const float TIEMPO_DIFERENCIA = 10000;
    };
    Constants consts;

    //TURN
    bool primera_vez_turn=true;
    int modo;
    //FOLLOW WALL
    bool primera_vez_fw=true;

    //int id_giraff;
};

#endif