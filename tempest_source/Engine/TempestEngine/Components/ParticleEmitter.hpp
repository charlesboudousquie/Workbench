/*!**********************************************************************************************************
\file       ParticleEmitter.hpp
\author     Cody Cannell
\date       11/10/2018
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			The origin point of the particles 
************************************************************************************************************/
#pragma once
#include "../Rendering/ShaderProgram.hpp"
#include "../Rendering/ParticleSystem.hpp"
#include "../SceneManagement/GameObject.hpp"
#include "../SceneManagement/Transform.hpp"
#include <Component.hpp>
#include <Vector4.hpp>
#include <Color4.hpp>

enum class emitterType
{
	enm_default = 0,
	enm_fire = 1,
};


enum class emitterShape
{
	enm_cube = 0,
	enm_circle = 1,
	enm_cone = 2,
	enm_torus = 3,
	enm_size,
};

//  particle creation position range/area 2D
//  color start color range/ end color range
//  

struct emitterData
{
    //std::string m_textureName;
    vector4 m_direction = { 0,1,0,0 };
	//color4 m_colorMin = { 1,0,0 };
	//color4 m_colorMax = { 0,0,1 };
	color4 m_startColRange = { 1,0,0 };
	color4 m_startColRange2 = { 1,0,0 };
	color4 m_endColRange = { 0,0,1 };
	color4 m_endColRange2 = { 0,0,1 };

    unsigned m_maxParticles = 1000;
    unsigned m_batchSize = 3;
    unsigned m_batchCount = 0; // 0 means looping forever
    unsigned m_interval = 6;
    float m_sizeMin = 3.0f;
    float m_sizeMax = 10.0f;
    float m_minForce = 5.0f;
    float m_maxForce = 10.0f;
    float m_minAcceleration = 2.0f;
    float m_maxAcceleration = 3.0f;
    float m_glowMin = .4f;
    float m_glowMax = .6f;
    float m_minLife = 30.0f;
    float m_maxLife = 40.0f;
	

	emitterShape m_shape = emitterShape::enm_circle;
	emitterType m_Type = emitterType::enm_default;

    bool m_on = true;

	mutable unsigned m_timeSinceLastInterval = 0;
};


class particleEmitter : public componentCRTP<particleEmitter>
{

public:

	   particleEmitter() = default;


		

    //====  Virtual/Overrides  ============================================================================//

    /*!***************************************************************************************
    \brief  Converts this into a type
    \return Type equivalent.
    *****************************************************************************************/
    typeRT toTypeRT() const override;

    componentType const type() const override { return getType(); }

    void updateFromTypeRT(typeRT& p_type) override;

		/*!***************************************************************************************
		\brief  Converts type to transform.
		\param p_type - type to convert
		*****************************************************************************************/
		particleEmitter(typeRT& p_type);

    static componentType const getType() { static componentType const type = componentType::forConcrete<particleEmitter>(); return type; }
		static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Particle Emitter", true, true, true); }

    void setEmitterUniforms(std::shared_ptr<shaderProgram> p_program)
    {

        matrix4x4 emitterModelMatrix = getGameObject().lock()->getComponent<transform>()->getGlobalMatrix();

        p_program->setUniforms("ModelMatrix", emitterModelMatrix);
    }

    //setters for particle data
    void setMaxParticles(unsigned p_maxParticles) { m_data.m_maxParticles = p_maxParticles;  m_isDirty = true; }

    const emitterData & getEmitterData() const { return m_data; }


    const int getBufferID() { return m_bufferID; };
    void setBefferID(int p_bufferID) { m_bufferID = p_bufferID; }

    void setDirtyFlag(bool p_flag) { m_isDirty = p_flag; }
    bool getDirtyFlag() { return m_isDirty; }

	bool getActiveFlag();
	void setActiveFlag(bool flag);

private:

	
   
    emitterData m_data; 
    bool m_isDirty = true;

    int m_bufferID = 0;
};
