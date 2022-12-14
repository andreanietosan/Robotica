//
// Created by gonzalo on 9/12/22.
//

#ifndef FORCEFIELD_DOOR_DETECTOR_H
#define FORCEFIELD_DOOR_DETECTOR_H

#include <Eigen/Dense>
#include <QtGui>
#include <QGraphicsItem>
#include <abstract_graphic_viewer/abstract_graphic_viewer.h>


class Door_detector
{
public:
    struct Door
    {
        Eigen::Vector2f p0, p1, p_center;
    };
    std::vector<Door> detector(const std::vector<Eigen::Vector2f> &line, AbstractGraphicViewer *viewer);
    void draw_doors(const std::vector<Door> &doors_v, AbstractGraphicViewer *viewer);
    void draw_peaks(std::vector<std::tuple<int, bool>> pecks, const std::vector<Eigen::Vector2f> &line, AbstractGraphicViewer *viewer);
};


#endif //FORCEFIELD_DOOR_DETECTOR_H