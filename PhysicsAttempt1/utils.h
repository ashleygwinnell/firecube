#ifndef MYUTIL_H
#define MYUTIL_H
float PointTriangleDistanceSquared(vec3 pos, vec3 v0, vec3 v1, vec3 v2, float &S, float &T);
float PointTriangleDistanceSquared(vec3 point, vec3 v0, vec3 v1, vec3 v2, vec3 &pointOnTriangle, bool &pointOnTriangleEdge);
void FindClosestTriangle(vec3 pos, const vector<vec3> &vertex, const vector<Face> &face, int &triangleIndex, vec3 &pointOnTriangle);
bool IntersectSegmentTriangle(vec3 start, vec3 length, vec3 v0, vec3 v1, vec3 v2, vec3 &pos, float &SegP, bool &segmentInDirOfTriangleNotmal);
int CountTriangleHits(vec3 start, vec3 length, const vector<vec3> &vertex, const vector<Face> &face);
#endif