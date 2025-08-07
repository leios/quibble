#include "../include/quibble.h"

quibble_point_1D qb_point_1D(float x){
    quibble_point_1D pt;
    pt.x = x;
    return pt;
}

quibble_point_2D qb_point_2D(float x, float y){
    quibble_point_2D pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

quibble_point_3D qb_point_3D(float x, float y, float z){
    quibble_point_3D pt;
    pt.x = x;
    pt.y = y;
    pt.z = z;
    return pt;
}

