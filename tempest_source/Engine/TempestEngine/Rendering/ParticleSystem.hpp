/*!***************************************************************************************
\file       ParticleSystem.hpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"
#include "Color.hpp"
#include "ShaderProgram.hpp"
#include "SimpleTexture.hpp"
#include "ParticleMesh.hpp"
#include "ParticleBuffer.hpp"
#include "../Components/Camera.hpp"
#include "../../EngineInterfaces/Public/IComponentRepository.hpp"

#include <random> // std::mt19937

#include <GL/gl.h>

enum class emitterShape;
class transform;
struct emitterData;

#define MAXPARTICLES 8000


struct cpu_particle
{
    cpu_particle() = default;
    cpu_particle(vector4 p_position, float p_size, color p_color) : m_position(p_position), m_color(p_color), m_size(p_size) {}
    
    vector4 m_position { 0,0,0,1 };
    vector4 m_velocity{ 0,0,0,0 };
    vector4 m_acceleration = { 0,0,0,0 };
    color m_color{ 1,0,0 };
    float m_size{ 0 };

    
    float m_lifeSpan { 100 };
    float m_life { -1.0f };

    float m_cameraDistance { -1.0f };

    bool operator<(cpu_particle& rhs) {
        // Sort in reverse order : far particles drawn first.
        return this->m_cameraDistance > rhs.m_cameraDistance;
    }
};


class particleSystem final : public systemBase
{
public:

    particleSystem() : m_particlePool(MAXPARTICLES) {}
    
    static const std::string& getName() { static const std::string n("particleSystem"); return n; }
    const std::string& name() const override { return getName(); }

protected:
        
    void onInitialize() override;
    void onUpdate() override;
    

private:

    void buildBuffers();
    void updateBuffers();
    void spawnParticles(const emitterData & l_emitterData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);
    void sortParticles();
    void update(float p_dt, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);
    void setUniforms();
    void render();
    


    std::shared_ptr<gameObject> getMainCamera();

    int findDeadParticles();

    void createNewParticle(unsigned p_index, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform);

	vector4 calculateRandomPositionBasedOnShapeType(std::mt19937 p_rng, emitterShape p_shape);


	bool isInsideCircle(float p_x, float p_y, float p_z);
	bool isInsideCone(float p_x, float p_y, float p_z);
	bool isInsideTorus(float p_x, float p_y, float p_z);


    
    std::shared_ptr<shaderProgram> m_shaderProgram;
  
    std::shared_ptr<simpleTexture> m_particleTexture;
 
    std::vector<cpu_particle> m_particlePool;

    int m_liveParticle = 0;

    particleMesh* m_mesh = nullptr;

    particleBuffer * m_buffer = nullptr;
    
    GLuint m_TextureID = 0;
};
