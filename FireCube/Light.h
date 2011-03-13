namespace FireCube
{

/**
* Specifies the type of a light.
*/
enum LightType
{
    DIRECTIONAL, POINT
};
/**
* A class storing various properties of a light source.
*/
class FIRECUBE_API Light
{
public:
    /**
    * Sets the light type.
    * @param type The new light type.
    */
    void SetType(LightType type);

    /**
    * Gets the type of this light.
    * @return The type of this light.
    */
    LightType GetType();

    /**
    * Sets the ambient color of this light.
    * @param color The ambient color to set.
    */
    void SetAmbientColor(vec4 color);

    /**
    * Gets the ambient color of this light.
    * @return The ambient color of this light.
    */
    vec4 GetAmbientColor();

    /**
    * Sets the diffuse color of this light.
    * @param color The diffuse color to set.
    */
    void SetDiffuseColor(vec4 color);

    /**
    * Gets the diffuse color of this light.
    * @return The diffuse color of this light.
    */
    vec4 GetDiffuseColor();

    /**
    * Sets the specular color of this light.
    * @param color The specular color to set.
    */
    void SetSpecularColor(vec4 color);

    /**
    * Gets the specular color of this light.
    * @return The specular color of this light.
    */
    vec4 GetSpecularColor();

    /**
    * Checks whether two lights are equal.
    * @return True if the two lights are equal, false otherwise.
    */
    bool operator == (const Light &other) const;

private:
    LightType type;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};
}