//
// Created by gonzalo on 9/12/22.
//

#include "door_detector.h"
#include <cppitertools/range.hpp>
#include <cppitertools/enumerate.hpp>
#include <cppitertools/filter.hpp>
#include <cppitertools/chunked.hpp>
#include <cppitertools/sliding_window.hpp>
#include <cppitertools/combinations_with_replacement.hpp>

////////////////////// DOOR DERECTOR //////////////////////////////////////////
std::vector<Door_detector::Door> Door_detector::detector(const std::vector<Eigen::Vector2f> &line, AbstractGraphicViewer *viewer)
{
    std::vector<float> derivaties(line.size() - 1);

    for(auto &&[i,d]:line | iter::sliding_window(2) | iter::enumerate)
        derivaties[i] = (d[1].norm() - d[0].norm());

    std::vector<std::tuple<int, bool>> pecks;

    for(auto &&[i,d]:derivaties | iter::enumerate)
    {
        if (d > 500) // Umbral
            pecks.push_back(std::make_tuple(i, true));
        else if (d < -500)
            pecks.push_back(std::make_tuple(i+1, false));
    }
    draw_peaks(pecks, line, viewer);
    std::vector<Door> doors;

    for(auto &&p:pecks | iter::combinations_with_replacement(2))
    {
        auto &[p1, pos1] = p[0];
        auto &[p2, pos2] = p[1];
        auto v1 = line[p1];
        auto v2 = line[p2];

        if(((pos1 and !pos2) or (pos2 and !pos1)) and ((v1 - v2).norm() < 1200 and (v1 - v2).norm() > 600) and (v1.y() > 200 and v2.y() > 200))
        {
            Door door{.p0=v1, .p1=v2, .p_center=(v1 + v2)/2};
            doors.push_back(door);
        }
    }
    return doors;
}
void Door_detector::draw_doors(const std::vector<Door> &doors_v, AbstractGraphicViewer *viewer)
{
    static std::vector<QGraphicsItem *> items;
    for(const auto &item: items)
        viewer->scene.removeItem(item);
    items.clear();

    for(auto &&[k, doors]: doors_v | iter::enumerate)
    {
        //qInfo() << __FUNCTION__ << k << (int)COLORS.row(k).x();
        QColor color("magenta");
        QBrush brush(color);
        QPen pen(color);
        for(const auto &p: doors_v)
        {
            auto item = viewer->scene.addEllipse(-100, -100, 200, 200, pen, brush);
            item->setPos(p.p_center.x(), p.p_center.y());
            items.push_back(item);
        }
    }
}

void Door_detector::draw_peaks(std::vector<std::tuple<int, bool>> pecks, const std::vector<Eigen::Vector2f> &line,
                               AbstractGraphicViewer *viewer)
{
    static std::vector<QGraphicsItem *> items;
    for(const auto &item: items)
        viewer->scene.removeItem(item);
    items.clear();
    QColor color("green");
    QBrush brush(color);
    QPen pen(color);

    for(auto &[p, b]: pecks)
    {
        auto item = viewer->scene.addEllipse(-100, -100, 200, 200, pen, brush);
        item->setPos(line[p].x(), line[p].y());
        items.push_back(item);
    }
}