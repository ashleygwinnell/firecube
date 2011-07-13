#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

namespace FireCube
{
/**
* A class representing an axis aligned bounding box.
*/
class FIRECUBE_API BoundingBox
{
public:
    BoundingBox();
    /**
    * Constructs a bounding box for two vertices.
    * @param min The minimum point of the bounding box
    * @param max The maximum point of the bounding box
    */
    BoundingBox(const vec3 &min, const vec3 &max);
    /**
    * Expands a bounding box to contain another.
    * @param bb The bounding box to expand with.
    */
    void Expand(const BoundingBox &bb);

    /**
    * Expands a bounding box to contain a vertex.
    * @param v The vertex to expand with.
    */
    void Expand(const vec3 &v);

    /**
    * @return The width of the bounding box.
    */
    float GetWidth() const;

    /**
    * @return The height of the bounding box.
    */
    float GetHeight() const;

    /**
    * @return The depth of the bounding box.
    */

    float GetDepth() const;
    /**
    * @return The center of the bounding box.
    */
    vec3 GetCenter() const;

	/**
    * Sets the minimum corner of this bounding box.
	* @param min The new minimum.
    */
    void SetMin(const vec3 &min);

    /**
    * @return The minimum point of the bounding box.
    */
    vec3 GetMin() const;

	/**
    * Sets the maximum corner of this bounding box.
	* @param max The new maximum.
    */
    void SetMax(const vec3 &max);

    /**
    * @return The maximum point of the bounding box.
    */
    vec3 GetMax() const;

    /**
    * Checks whether this bounding box entirely contains another one.
    * @param bb The bounding box to check.
    * @return True if the given bounding box is entirely contained inside this one, false otherwise.
    */
    bool Contains(const BoundingBox &bb) const;

	/**
	* Transforms the bounding box by a given 4x4 matrix.
	* @param mat The matrix to transform with.
	*/
	void Transform(const mat4 &mat);

	/**
	* @return The size of this bounding box.
	*/
	vec3 GetSize() const;

private:
    vec3 bmin, bmax;
};
}
#endif