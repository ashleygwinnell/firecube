#ifndef SHADERS_H
#define SHADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{

// Forward declarations.
template<class T>
class ResourceManager;
class Buffer;
typedef boost::shared_ptr<Buffer> BufferPtr;
class Program;

/**
* A shared pointer to a Program.
*/
typedef boost::shared_ptr<Program> ProgramPtr;

namespace Renderer
{
void FIRECUBE_API UseProgram(ProgramPtr program);
}

/**
* Specifies the type of a shader.
*/
enum ShaderType
{
    VERTEX_SHADER, FRAGMENT_SHADER
};

/**
* A class representing a single shader.
*/
class FIRECUBE_API Shader
{
    friend class Program;
    friend class ResourceManager<Shader>;
public:
    Shader();
    ~Shader();

    /**
    * Loads a shader from a file. The shader type is determined by the extension of the file: .vert for a vertex shader, .frag for a fragment shader.
    * @param filename The file to load.
    */
    bool Load(const std::string &filename);
    /**
    * Creates a shader from source.
    * @param type The type of the shader.
    * @param source The source of the shader.
    */
    bool Create(ShaderType type, const std::string &source);
    /**
    * Returns the resource id of the shader.
    */
    unsigned int GetId() const;

private:
    GLuint id;
};

/**
* A shared pointer to a Shader.
*/
typedef boost::shared_ptr<Shader> ShaderPtr;

/**
* A shader resource manager.
*/
typedef ResourceManager<Shader> ShaderManager;

/**
* A class representing a gpu program.
*/
class FIRECUBE_API Program
{
    friend void Renderer::UseProgram(ProgramPtr program);    
public:
    Program();
    ~Program();

    /**
    * Creates the program.
    */
    void Create();

    /**
    * Creates the program and attaches two shaders to it.
    */
    void Create(ShaderPtr shader1, ShaderPtr shader2);

    /**
    * Attaches a shader to the program.
    * @param shader The shader to attach.
    */
    void Attach(ShaderPtr shader);

    /**
    * Links the program.
    */
    void Link();

    /**
    * Sets a 1d uniform float.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, float value);

    /**
    * Sets a 1d uniform integer.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, int value);

    /**
    * Sets a 2d uniform float.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const vec2 &value);

    /**
    * Sets a 3d uniform float.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const vec3 &value);

    /**
    * Sets a 4d uniform float.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const vec4 &value);

    /**
    * Sets a 3x3 uniform matrix.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const mat3 &value);

    /**
    * Sets a 4x4 uniform matrix.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const mat4 &value);

    /**
    * Sets a 1d uniform boolean.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, bool value);

    /**
    * Sets an array of booleans.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const std::vector<bool> &value);

    /**
    * Sets an array of integers.
    * @param name The name of the variable.
    * @param value The value to assign for it.
    */
    void SetUniform(const std::string &name, const std::vector<int> &value);

    /**
    * Sets vertex shader attribute.
    * @param name The name of the variable.
    * @param buffer The buffer from which data will be read.
    * @param size The number of elements in each attribue.
    */
    void SetAttribute(const std::string &name, BufferPtr buffer, int size);

    /**
    * @return Returns the compile log for this program.
    */
    std::string GetInfoLog() const;

    /**
    * Returns whether the program is valid.
    */
    bool IsValid() const;

    /**
    * Returns the resource id of the program.
    */
    unsigned int GetId() const;

private:
    GLuint id;
    std::map<std::string, GLint> variables;
};

class ShaderProperties;
/**
* A class representing a technique.
*/
class FIRECUBE_API Technique
{
public:

    /**
    * Loads the specified shader.
    * @param filename The file name of the shader.
    * @return True on success, false otherwise.
    */
    bool LoadShader(const std::string &filename);

    /**
    * Loads the specified shader.
    * @param type The type of the shader.
    * @param source The source of the shader.
    * @return True on success, false otherwise.
    */
    bool LoadShader(ShaderType type, const std::string &source);

    /**
    * Generates a program from a given shader properties.
    * @param shaderProperties The shader properties to use to generate the program.
    */
    ProgramPtr GenerateProgram(const ShaderProperties &shaderProperties);

private:
    std::map<unsigned int, ProgramPtr> programs;
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
};

/**
* A shared pointer to a Technique.
*/
typedef boost::shared_ptr<Technique> TechniquePtr;

/**
* A class storing a list of uniforms to be fed into a gpu program.
*/
class FIRECUBE_API ProgramUniformsList
{
public:

    /**
    * Assigns an integer value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetIntValue(const std::string &name, int value);

    /**
    * Gets the value of an integer uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    int GetIntValue(const std::string &name) const;

    /**
    * Assigns a float value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetFloatValue(const std::string &name, float value);

    /**
    * Gets the value of a float uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    float GetFloatValue(const std::string &name) const;

    /**
    * Assigns a 2d vector value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetVec2Value(const std::string &name, const vec2 &value);

    /**
    * Gets the value of a 2d vector uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    vec2 GetVec2Value(const std::string &name) const;

    /**
    * Assigns a 3d vector value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetVec3Value(const std::string &name, const vec3 &value);

    /**
    * Gets the value of a 3d vector uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    vec3 GetVec3Value(const std::string &name) const;

    /**
    * Assigns a 4d vector value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetVec4Value(const std::string &name, const vec4 &value);

    /**
    * Gets the value of a 4d vector uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    vec4 GetVec4Value(const std::string &name) const;

    /**
    * Assigns a 3x3 matrix value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetMat3Value(const std::string &name, const mat3 &value);

    /**
    * Gets the value of a 3x3 matrix uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    mat3 GetMat3Value(const std::string &name) const;

    /**
    * Assigns a 4x4 matrix value to a named uniform variable.
    * @param name The name of the uniform variable.
    * @param value The value to assign.
    */
    void SetMat4Value(const std::string &name, const mat4 &value);

    /**
    * Gets the value of a 4x4 matrix uniform variable.
    * @param name The name of the uniform variable.
    * @return The value of the uniform variable.
    */
    mat4 GetMat4Value(const std::string &name) const;

    /**
    * Removes a variable from the list.
    * @param name The name of the variable to remove.
    */
    void RemoveValue(const std::string &name);

    /**
    * Applies the list to a gpu program.
    * @param program The program to apply the list to.
    */
    void ApplyForProgram(ProgramPtr program) const;

private:
    std::map<std::string, int> intMap;
    std::map<std::string, float> floatMap;
    std::map<std::string, vec2> vec2Map;
    std::map<std::string, vec3> vec3Map;
    std::map<std::string, vec4> vec4Map;
    std::map<std::string, mat3> mat3Map;
    std::map<std::string, mat4> mat4Map;
};

}
#pragma warning(pop)
#endif