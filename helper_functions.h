#pragma once
#include <Eigen/Eigen>

//Pineda's Edge detection function
//Checks if a point is inside or outside a triangle
bool check_if_point_in_triangle(Eigen::Vector3f v0, Eigen::Vector3f v1, Eigen::Vector3f v2,
    Eigen::Vector3f p)
{
    //the order of vertices matter
    float E01 = (p[0] - v0[0]) * (v1[1] - v0[1]) - (p[1] - v0[1]) * (v1[0] - v0[0]);
    float E12 = (p[0] - v1[0]) * (v2[1] - v1[1]) - (p[1] - v1[1]) * (v2[0] - v1[0]);
    float E20 = (p[0] - v2[0]) * (v0[1] - v2[1]) - (p[1] - v2[1]) * (v0[0] - v2[0]);

    if (E01 >= 0 && E12 > 0 && E20>=0)
    {
        return true;
    }
    return false;
}