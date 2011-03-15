#include <FireCube.h>
using namespace std;
using namespace FireCube;
inline float MyAbs(float v)
{
    return v < 0 ? -v : v;
}
float PointTriangleDistanceSquared(vec3 pos, vec3 v0, vec3 v1, vec3 v2, float &S, float &T)
{

    vec3 kDiff = v0 - pos;
    float fA00 = (v1 - v0).Length2();
    float fA01 = Dot(v1 - v0, v2 - v0);
    float fA11 = (v2 - v0).Length2();
    float fB0 = Dot(kDiff, v1 - v0);
    float fB1 = Dot(kDiff, v2 - v0);
    float fC = kDiff.Length2();
    float fDet = MyAbs(fA00 * fA11 - fA01 * fA01);
    float fS = fA01 * fB1 - fA11 * fB0;
    float fT = fA01 * fB0 - fA00 * fB1;
    float fSqrDist;

    if ( fS + fT <= fDet )
    {
        if ( fS < (float)0.0 )
        {
            if ( fT < (float)0.0 )  // region 4
            {
                if ( fB0 < (float)0.0 )
                {
                    fT = (float)0.0;
                    if ( -fB0 >= fA00 )
                    {
                        fS = (float)1.0;
                        fSqrDist = fA00 + ((float)2.0) * fB0 + fC;
                    }
                    else
                    {
                        fS = -fB0 / fA00;
                        fSqrDist = fB0 * fS + fC;
                    }
                }
                else
                {
                    fS = (float)0.0;
                    if ( fB1 >= (float)0.0 )
                    {
                        fT = (float)0.0;
                        fSqrDist = fC;
                    }
                    else if ( -fB1 >= fA11 )
                    {
                        fT = (float)1.0;
                        fSqrDist = fA11 + ((float)2.0) * fB1 + fC;
                    }
                    else
                    {
                        fT = -fB1 / fA11;
                        fSqrDist = fB1 * fT + fC;
                    }
                }
            }
            else  // region 3
            {
                fS = (float)0.0;
                if ( fB1 >= (float)0.0 )
                {
                    fT = (float)0.0;
                    fSqrDist = fC;
                }
                else if ( -fB1 >= fA11 )
                {
                    fT = (float)1.0;
                    fSqrDist = fA11 + ((float)2.0) * fB1 + fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDist = fB1 * fT + fC;
                }
            }
        }
        else if ( fT < (float)0.0 )  // region 5
        {
            fT = (float)0.0;
            if ( fB0 >= (float)0.0 )
            {
                fS = (float)0.0;
                fSqrDist = fC;
            }
            else if ( -fB0 >= fA00 )
            {
                fS = (float)1.0;
                fSqrDist = fA00 + ((float)2.0) * fB0 + fC;
            }
            else
            {
                fS = -fB0 / fA00;
                fSqrDist = fB0 * fS + fC;
            }
        }
        else  // region 0
        {
            // minimum at interior point
            float fInvDet = ((float)1.0) / fDet;
            fS *= fInvDet;
            fT *= fInvDet;
            fSqrDist = fS * (fA00 * fS + fA01 * fT + ((float)2.0) * fB0) +
                       fT * (fA01 * fS + fA11 * fT + ((float)2.0) * fB1) + fC;
        }
    }
    else
    {
        float fTmp0, fTmp1, fNumer, fDenom;

        if ( fS < (float)0.0 )  // region 2
        {
            fTmp0 = fA01 + fB0;
            fTmp1 = fA11 + fB1;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if ( fNumer >= fDenom )
                {
                    fS = (float)1.0;
                    fT = (float)0.0;
                    fSqrDist = fA00 + ((float)2.0) * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = (float)1.0 - fS;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((float)2.0) * fB1) + fC;
                }
            }
            else
            {
                fS = (float)0.0;
                if ( fTmp1 <= (float)0.0 )
                {
                    fT = (float)1.0;
                    fSqrDist = fA11 + ((float)2.0) * fB1 + fC;
                }
                else if ( fB1 >= (float)0.0 )
                {
                    fT = (float)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fT = -fB1 / fA11;
                    fSqrDist = fB1 * fT + fC;
                }
            }
        }
        else if ( fT < (float)0.0 )  // region 6
        {
            fTmp0 = fA01 + fB1;
            fTmp1 = fA00 + fB0;
            if ( fTmp1 > fTmp0 )
            {
                fNumer = fTmp1 - fTmp0;
                fDenom = fA00 - ((float)2.0) * fA01 + fA11;
                if ( fNumer >= fDenom )
                {
                    fT = (float)1.0;
                    fS = (float)0.0;
                    fSqrDist = fA11 + ((float)2.0) * fB1 + fC;
                }
                else
                {
                    fT = fNumer / fDenom;
                    fS = (float)1.0 - fT;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + ((float)2.0) * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((float)2.0) * fB1) + fC;
                }
            }
            else
            {
                fT = (float)0.0;
                if ( fTmp1 <= (float)0.0 )
                {
                    fS = (float)1.0;
                    fSqrDist = fA00 + ((float)2.0) * fB0 + fC;
                }
                else if ( fB0 >= (float)0.0 )
                {
                    fS = (float)0.0;
                    fSqrDist = fC;
                }
                else
                {
                    fS = -fB0 / fA00;
                    fSqrDist = fB0 * fS + fC;
                }
            }
        }
        else  // region 1
        {
            fNumer = fA11 + fB1 - fA01 - fB0;
            if ( fNumer <= (float)0.0 )
            {
                fS = (float)0.0;
                fT = (float)1.0;
                fSqrDist = fA11 + ((float)2.0) * fB1 + fC;
            }
            else
            {
                fDenom = fA00 - 2.0f * fA01 + fA11;
                if ( fNumer >= fDenom )
                {
                    fS = (float)1.0;
                    fT = (float)0.0;
                    fSqrDist = fA00 + ((float)2.0) * fB0 + fC;
                }
                else
                {
                    fS = fNumer / fDenom;
                    fT = (float)1.0 - fS;
                    fSqrDist = fS * (fA00 * fS + fA01 * fT + ((float)2.0) * fB0) +
                               fT * (fA01 * fS + fA11 * fT + ((float)2.0) * fB1) + fC;
                }
            }
        }
    }

    S = fS;
    T = fT;

    return MyAbs(fSqrDist);
}

float PointTriangleDistanceSquared(vec3 point, vec3 v0, vec3 v1, vec3 v2, vec3 &pointOnTriangle, bool &pointOnTriangleEdge)
{
    float S, T;

    float dist2 = PointTriangleDistanceSquared(point, v0, v1, v2, S, T);
    pointOnTriangle = v0 + S * (v1 - v0) + T * (v2 - v0);

    const float epsilon = 0.00001f;
    if ( (S < epsilon) ||
            (T < epsilon) ||
            ( (S + T) > (0.5f - epsilon) ) )
        pointOnTriangleEdge = true;
    else
        pointOnTriangleEdge = false;

    return dist2;
}
void FindClosestTriangle(vec3 pos, const vector<vec3> &vertex, const vector<Face> &face, int &triangleIndex, vec3 &pointOnTriangle)
{
    int i;
    const int num = face.size();
    vec3 thisPoint;
    float closestDistance = 1.0E10;
    bool found = false;
    bool pointOnTriangleEdge;

    for (i = 0 ; i < num ; i++)
    {
        float dist = PointTriangleDistanceSquared(pos, vertex[face[i].v[0]], vertex[face[i].v[1]], vertex[face[i].v[2]], thisPoint, pointOnTriangleEdge);
        if (dist < closestDistance)
        {
            found = true;
            triangleIndex = i;
            pointOnTriangle = thisPoint;
            closestDistance = dist;
        }
    }
}
bool IntersectSegmentTriangle(vec3 start, vec3 length, vec3 v0, vec3 v1, vec3 v2, vec3 &pos, float &SegP, bool &segmentInDirOfTriangleNotmal)
{
    vec3 triNormal = Cross(v1 - v0, v2 - v0);
    triNormal.Normalize();
    float triDist = -Dot(triNormal, v0);

    float perp_dist_seg0 = Dot(start, triNormal) + triDist;
    float perp_dist_seg1 = Dot(start + length, triNormal) + triDist;

    if (perp_dist_seg0 * perp_dist_seg1 >= 0.0f)
        return false; // both on same side.

    SegP = (perp_dist_seg0 / (perp_dist_seg0 - perp_dist_seg1));

    pos = start + SegP * length;

    // check each triangle edge plane in turn. Each plane normal points out.

    // edge 0
    vec3 planeNormal = Cross(v1 - v0, triNormal).Normalize();
    float planeDist = -Dot(planeNormal, v0);

    float pointDist = Dot(pos, planeNormal) + planeDist;
    if (pointDist > 0.0f)
        return false;

    // edge 1
    planeNormal = Cross(triNormal, v2 - v0).Normalize();
    planeDist = -Dot(planeNormal, v0);

    pointDist = Dot(pos, planeNormal) + planeDist;
    if (pointDist > 0.0f)
        return false;

    // the other edge - from the end of edge0 to the end of edge 1
    vec3 edge = v2 - v1;
    planeNormal = Cross(edge, triNormal).Normalize();
    planeDist = -Dot(planeNormal, v1);

    pointDist = Dot(pos, planeNormal) + planeDist;
    if (pointDist > 0.0f)
        return false;

    // point is in triangle! just set the direction flag
    segmentInDirOfTriangleNotmal = (perp_dist_seg0 < 0.0f);

    return true;
}
int CountTriangleHits(vec3 start, vec3 length, const vector<vec3> &vertex, const vector<Face> &face)
{
    int i;
    const int num = face.size();
    int hits = 0;
    vec3 pos;
    float S;
    bool segInDir;
    for (i = 0 ; i < num ; ++i)
    {
        if (IntersectSegmentTriangle(start, length, vertex[face[i].v[0]], vertex[face[i].v[1]], vertex[face[i].v[2]], pos, S, segInDir))
        {
            ++hits;
        }
    }
    return hits;
}