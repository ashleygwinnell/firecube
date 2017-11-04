#pragma once

#include "Scene/Renderable.h"

namespace FireCube
{

class Shader;

enum class ParticleEmitterShape
{
	BOX, SPHERE
};

enum class ParticleEmitterSimulationSpace
{
	LOCAL, WORLD
};

/**
* This class represents a particle emitter component. Particles are rendered as point sprites
*/
class ParticleEmitter : public Renderable
{
	FIRECUBE_OBJECT(ParticleEmitter)
public:

	/**
	* Constructs the particle emitter.
	* @pram engine The engine
	* @param numberOfParticles The maximum number of particles
	* @param material The material to use to render the particles
	*/
	ParticleEmitter(Engine *engine, unsigned int numberOfParticles, Material *material);
	~ParticleEmitter();

	virtual void IntersectRay(RayQuery &rayQuery);
	
	/**
	* Resets the particle emitter to its initial state
	*/
	void Reset();
	
	/**
	* Clones this particle emitter
	*/
	virtual Component *Clone() const;
	
	/**
	* Sets the material to use the render the particles
	* @param material The material to use
	*/
	void SetMaterial(Material *material);
	
	/**
	* @returns The material used to render the particles
	*/
	Material *GetMaterial() const;
	
	/**
	* Sets the bounding box of the emitter. This box is used for frustum culling and should be large enough to allways contain tll the particles
	*/
	void SetBoundingBox(BoundingBox boundingBox);
	
	/**
	* Sets the emitting shape to a box
	* @param box The box extents
	*/	
	void SetBoxEmitter(vec3 box);
	
	/**
	* Sets the emitting shape to a sphere
	* @param radius The radius of the sphere
	*/
	void SetSphereEmitter(float radius);
	
	/**
	* Sets the emission rate of new particles (number of particles per second)
	* @param emissionRate The emission rate
	*/
	void SetEmissionRate(unsigned int emissionRate);

	/**
	* Gets the emission rate of new particles (number of particles per second)	
	*/
	unsigned int GetEmissionRate() const;
	
	/**
	* Sets the life span of particles (in seconds)
	* @param minLifeTime the minimum life time of particles
	* @param maxLifeTime The maximum life time of particles
	*/
	void SetLifeTime(float minLifeTime, float maxLifeTime);
	
	/**
	* Sets the speed of particles
	* @param minSpeed The minimum speed of particles
	* @param maxSpeed The maximum speed of particles
	*/
	void SetSpeed(float minSpeed, float maxSpeed);
	
	/**
	* Renders debug geometry of this particle emitter
	* @param debugRenderer The debug renderer
	*/
	virtual void RenderDebugGeometry(DebugRenderer *debugRenderer);
	
	/**
	* Sets whether to preward the system. If prewarming is enabled, the particles are simulated a whole cycle before being used
	* @param preward Whether to preward the system
	*/
	void SetPrewarm(bool prewarm);
	
	/**
	* Sets the simulation space of the particles. If the particles are simulated in world space, the transformation of the parent node doesn't affect particles
	* after they are spawned
	* @param simulationSpace The simulation space to use (either local or world)
	*/
	void SetSimulationSpace(ParticleEmitterSimulationSpace simulationSpace);
	
	/**
	* @returns The simulation space of particles
	*/
	ParticleEmitterSimulationSpace GetSimulationSpace() const;

	void SetNumberOfParticles(unsigned int numberOfParticles);

protected:	
	virtual void UpdateWorldBoundingBox();
	virtual void UpdateRenderableParts();	

private:

	const int particleDataSize = 8;

	ParticleEmitter(const ParticleEmitter &other);
	void Update(float dt);
	void EmitParticles(unsigned int count);
	void Init(unsigned int numberOfParticles, Material *material);
	inline void RandomPositionAndDirection(vec3 &position, vec3 &direction) const;	
	void Prewarm();
	void UpdateBoundingBox();

	BoundingBox boundingBox;
	Shader *updateShader;
	VertexBuffer *particleBuffers[2];	
	SharedPtr<Geometry> geometry;
	float minLifeTime;
	float maxLifeTime;
	float minSpeed;
	float maxSpeed;
	unsigned int numberOfParticles;
	std::vector<float> particleLife;
	std::vector<unsigned int> deadParticles;
	unsigned int emissionRate;
	bool needToReset;
	ParticleEmitterShape emitterShape;
	float radius;
	vec3 box;
	float emissionLeftOver;
	bool prewarm;
	ParticleEmitterSimulationSpace simulationSpace;
	SharedPtr<Material> material;
};

}