#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <sstream>
#include <limits>
using namespace std;
#include <SDL.h>
#include <windows.h>
#include "GLee.h"

#include "utils.h"
#include "MyMath.h"
#include "BoundingBox.h"


using namespace FireCube;

BoundingBox::BoundingBox() : bmin((std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)(), (std::numeric_limits<float>::max)()), bmax(-(std::numeric_limits<float>::max)(), -(std::numeric_limits<float>::max)(), -(std::numeric_limits<float>::max)())
{
}
BoundingBox::BoundingBox(const vec3 &min, const vec3 &max)
{
    bmin = min;
    bmax = max;
}
void BoundingBox::Expand(const BoundingBox &bb)
{
    bmin.x = min(bmin.x, bb.bmin.x);
    bmin.y = min(bmin.y, bb.bmin.y);
    bmin.z = min(bmin.z, bb.bmin.z);

    bmax.x = max(bmax.x, bb.bmax.x);
    bmax.y = max(bmax.y, bb.bmax.y);
    bmax.z = max(bmax.z, bb.bmax.z);
}
void BoundingBox::Expand(const vec3 &v)
{
    bmin.x = min(bmin.x, v.x);
    bmin.y = min(bmin.y, v.y);
    bmin.z = min(bmin.z, v.z);

    bmax.x = max(bmax.x, v.x);
    bmax.y = max(bmax.y, v.y);
    bmax.z = max(bmax.z, v.z);
}
float BoundingBox::GetWidth()
{
    return bmax.x - bmin.x;
}
float BoundingBox::GetHeight()
{
    return bmax.y - bmin.y;
}
float BoundingBox::GetDepth()
{
    return bmax.z - bmin.z;
}
vec3 BoundingBox::GetCenter()
{
    return (bmin + bmax) / 2.0f;
}
bool BoundingBox::Contains(const BoundingBox &bb)
{
    if (bb.bmin.x >= bmin.x && bb.bmin.y >= bmin.y && bb.bmin.z >= bmin.z && bb.bmax.x <= bmax.x &&  bb.bmax.y <= bmax.y &&  bb.bmax.z <= bmax.z)
        return true;
    return false;
}
vec3 BoundingBox::GetMin()
{
    return bmin;
}
vec3 BoundingBox::GetMax()
{
    return bmax;
}