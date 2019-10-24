/*!***************************************************************************************
\file       ParticleSystem.cpp
\author     Cody Cannell
\date       7/31/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#include "../Precompiled.h"
#include "ParticleSystem.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include "../SystemManager.hpp"
#include "../Components/ParticleEmitter.hpp"
#include "TypeData.hpp"
#include "ShaderProgram.hpp"
#include "GraphicsSystem.hpp"
#include "../Components/Camera.hpp"
#include "MeshBuilder.hpp"
#include "ParticleMesh.hpp"





void particleSystem::onInitialize()
{
    // collect configuration
    std::string l_assetPath = this->getSystemManager()->getConfigurationFunction()().asset_path.generic_string();

    // set up particle shaderPrograme
    m_shaderProgram = std::make_shared<shaderProgram>(assetManager::getAsset(L"shaders/vertex/Particle.vert"), assetManager::getAsset(L"shaders/fragment/Particle.frag"));
    m_shaderProgram->build();
    m_shaderProgram->bind();


    // set up textures
    m_particleTexture = std::make_shared<simpleTexture>(assetManager::getAsset(L"textures/Health.png"));
    m_particleTexture->build();
    m_particleTexture->bind(0);

		m_shaderProgram->bind();

    m_shaderProgram->setUniforms("Texture", m_particleTexture->getBoundSlot());

    // create particle container
    m_mesh= new particleMesh();

    m_buffer = new particleBuffer(MAXPARTICLES, m_mesh->getParticleSize());
    m_buffer->build(m_mesh);
    m_buffer->bind();



}

void particleSystem::onUpdate()
{
    GameObjectFiltering::componentTypeSet l_patternEmitter;
    l_patternEmitter.setType(particleEmitter::getType());
    auto l_listEmitters = getSystemManager()->getGameObjectGatherer()->getList(l_patternEmitter);

    /*GLint programId;
    glGetIntegerv(GL_PROGRAM_BINDING_ARB, &programId);*/

    std::shared_ptr<gameObject> l_camera = getMainCamera();

    m_shaderProgram->bind();


    //set up a particle pool
    for(auto & l_emitterObject : l_listEmitters)
    {
        auto l_emitter = l_emitterObject->getComponent<particleEmitter>();
			if(!l_emitter->getEmitterData().m_on)
			{
				continue;
			}
        auto l_emitterTransform = l_emitterObject->getComponent<transform>();
        auto l_cameraTransform = l_camera->getComponent<transform>();



        // get the buffer for this emitter
        const int bufferID = l_emitter->getBufferID();

        // get the new data for this emitter and create new particles with it
        spawnParticles(l_emitter->getEmitterData(), l_emitterTransform, l_cameraTransform);

        // updates the cpu particles and put fill the buffer object
        update(getDt(), l_emitter->getEmitterData(), l_emitterTransform, l_cameraTransform);

        //update gpu buffers with new particle
        updateBuffers();
        //
        l_camera->getComponent<camera>()->setCameraUniforms(m_shaderProgram);
        l_emitter->setEmitterUniforms(m_shaderProgram);


        render();

    }
}

void particleSystem::buildBuffers()
{


}

void particleSystem::updateBuffers()
{
    m_buffer->update();
}

void particleSystem::spawnParticles(const emitterData & p_emitterData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{

    unsigned l_interval = p_emitterData.m_interval;



    if (p_emitterData.m_timeSinceLastInterval < l_interval)
    {
			p_emitterData.m_timeSinceLastInterval++;
    }
    else
    {
        unsigned l_batchSize = p_emitterData.m_batchSize;

        for (unsigned i = 0; i < l_batchSize; ++i)
        {
          int l_index = findDeadParticles();

          if(l_index >= 0)
             createNewParticle(l_index, p_emitterData, p_emiterTransform, p_cameraTransform);

        }


        sortParticles();
				p_emitterData.m_timeSinceLastInterval = 0;
    }


}

void particleSystem::sortParticles()
{
    std::sort(&m_particlePool[0], &m_particlePool[MAXPARTICLES - 1]);
}

void particleSystem::update(float p_dt, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{
    UNREFERENCED_PARAMETER(p_eData);
    UNREFERENCED_PARAMETER(p_emiterTransform);
   // auto inverseEmitterMatrix = p_emiterTransform->getGlobalMatrix().inverse();
    m_buffer->resetParticleCount();
    int & l_particlesCount = m_buffer->getParticleCount();

    for (int i = 0; i < MAXPARTICLES; i++) {

        cpu_particle& p = m_particlePool[i];

        if (p.m_life > 0.0f) {

            // Decrease life
            p.m_life -= p_dt;
            if (p.m_life > 0.0f) {

                // Simulate simple physics : gravity only, no collisions
                p.m_acceleration += vector4(0.0f, -9.81f, 0.0f, 0.0f) * p_dt * 0.5f;
                p.m_position += p.m_acceleration * p_dt;

                 // eugler integration step
                p.m_velocity += p.m_acceleration * p_dt;
                p.m_position += p.m_velocity * p_dt;

                vector4 distanceVector = p.m_position - vector4(p_cameraTransform->getGlobalPosition(), 1);
                p.m_cameraDistance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y + distanceVector.z * distanceVector.z);



                //logger("particleSystem").debug() << "Particle: " << i << "Position: " << p.m_position.toStringPtr() << "\n";

               // p.m_position = inverseEmitterMatrix * p.m_position;

                // Fill the GPU buffer
                m_buffer->addParticle(l_particlesCount, particleMesh::gpu_particle({ p.m_position.x, p.m_position.y, p.m_position.z, p.m_size },
                                                                 { p.m_color.x,p.m_color.y,p.m_color.z,p.m_color.w }));
            }
            else {
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.m_cameraDistance = -1.0f;
                m_liveParticle--;
            }

            l_particlesCount++;

        }


    }

    // Debug
    /*for(size_t i = 0; i < m_particlePool.size(); ++i)
    {
        logger("particleSystem").debug() << "Particle: " << i << " Position: " << m_particlePool[i].m_position.toStringPtr() << " Life: " << m_particlePool[i].m_life;
    }*/
}

void particleSystem::setUniforms()
{

}

void particleSystem::render()
{
    m_buffer->render();
}

std::shared_ptr<gameObject> particleSystem::getMainCamera()
{
    GameObjectFiltering::componentTypeSet l_patternCamera;
    l_patternCamera.setType(camera::getType());
    auto l_listCameras = getSystemManager()->getGameObjectGatherer()->getList(l_patternCamera);

    for(auto p_camera : l_listCameras)
    {
        if(p_camera->getName() == "Main Camera")
        {
            return p_camera;
        }
    }

    return nullptr;
}

int particleSystem::findDeadParticles()
{
    for (int i = m_liveParticle; i < MAXPARTICLES; i++) {
        if (m_particlePool[i].m_life < 0) {
            return i;
        }
    }

    for (int i = 0; i < m_liveParticle; i++) {
        if (m_particlePool[i].m_life < 0) {
            return i;
        }
    }


    return -1; // All particles are taken, override the first one
}

void particleSystem::createNewParticle(unsigned p_index, emitterData p_eData, componentHandle<transform> p_emiterTransform, componentHandle<transform> p_cameraTransform)
{
    // get particle
    cpu_particle& p = m_particlePool[p_index];

    auto l_emitter_matrix = p_emiterTransform->getGlobalMatrix();
    auto l_emitter_direction = vector4(p_emiterTransform->getForward(), 1);
		auto l_shape_orientation = p_eData.m_direction;

    // set up random number gereator
    std::mt19937 rng;
    rng.seed(std::random_device()());


	vector4 l_randPos = calculateRandomPositionBasedOnShapeType(rng, p_eData.m_shape);



    p.m_position = l_emitter_matrix * l_randPos;

		if(p_eData.m_sizeMin <= p_eData.m_sizeMax)
		{
			std::uniform_real_distribution<float> randomSize(p_eData.m_sizeMin, p_eData.m_sizeMax);
			p.m_size = randomSize(rng);
		}

		if (p_eData.m_minLife <= p_eData.m_maxLife)
		{
			std::uniform_real_distribution<float> randomLife(p_eData.m_minLife, p_eData.m_maxLife);
			p.m_life = randomLife(rng);
		}

		if (p_eData.m_minForce <= p_eData.m_maxForce)
		{
			std::uniform_real_distribution<float> randomForce(p_eData.m_minForce, p_eData.m_maxForce);
			p.m_velocity = vector4(l_emitter_direction * randomForce(rng));
		}

		if (p_eData.m_minAcceleration <= p_eData.m_maxAcceleration)
		{
			std::uniform_real_distribution<float> randomAccel(p_eData.m_minAcceleration, p_eData.m_maxAcceleration);
			p.m_acceleration = vector4(l_emitter_direction *randomAccel(rng));
		}


    vector4 distanceVector = p.m_position - vector4(p_cameraTransform->getGlobalPosition(), 1);
    p.m_cameraDistance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y + distanceVector.z * distanceVector.z);

    m_liveParticle++;
}


vector4 particleSystem::calculateRandomPositionBasedOnShapeType(std::mt19937 p_rng, emitterShape p_shape)
{

	while(true)
	{
		// find a random location in the Emitter cube
		std::uniform_real_distribution<float> randomX(-1.0f, 1.0f);
		std::uniform_real_distribution<float> randomY(-1.0f, 1.0f);
		std::uniform_real_distribution<float> randomZ(-1.0f, 1.0f);

		float l_x = randomX(p_rng);
		float l_y = randomY(p_rng);
		float l_z = randomZ(p_rng);

		// prune by shape type
		switch (p_shape)
		{
			case emitterShape::enm_cube:

				return vector4(l_x, l_y, l_z, 1);

			case emitterShape::enm_circle:

				if(isInsideCircle(l_x,l_y,l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;

			case emitterShape::enm_cone:

				if (isInsideCone(l_x, l_y, l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;

			case emitterShape::enm_torus:

				if (isInsideTorus(l_x, l_y, l_z))
					return vector4(l_x, l_y, l_z, 1);
				else
					continue;
			case emitterShape::enm_size:
			default:
				// TODO: Log a warning, or deal with this properly
				;
		}
	}

}

bool particleSystem::isInsideCircle(float p_x, float p_y, float p_z)
{
	return (p_x * p_x + p_y * p_y + p_z * p_z) <= 1;
}

bool particleSystem::isInsideCone(float p_x, float p_y, float p_z)
{
		// put y into 0 to 1 range which becomes radius at height y
		float l_r = 1.0f - ((p_y + 1.0f) / 2.0f);

		// is inside radius
		return (p_x* p_x + p_z * p_z) <= l_r;
}

bool particleSystem::isInsideTorus(float p_x, float p_y, float p_z)
{
	//(x ^ 2 + y ^ 2 + z ^ 2 - (a ^ 2 + b ^ 2)) ^ 2 - 4 * a*b*(b ^ 2 - z ^ 2)

	// minor radius
	float l_r1 = 1;

	// major radius
	float l_r2 = 2;

	return ((p_x * p_x + p_y + p_y + p_z + p_z) - (l_r1 * l_r1 + l_r2 * l_r2)) *
		((p_x * p_x + p_y + p_y + p_z + p_z) - (l_r1 * l_r1 + l_r2 * l_r2)) - 4.0f * l_r1 * l_r2 * (l_r2 * l_r2 - p_z * p_z) < 0;


}

//const uint8_t HSVlights[61] =
//{ 0, 4, 8, 13, 17, 21, 25, 30, 34, 38, 42, 47, 51, 55, 59, 64, 68, 72, 76,
//81, 85, 89, 93, 98, 102, 106, 110, 115, 119, 123, 127, 132, 136, 140, 144,
//149, 153, 157, 161, 166, 170, 174, 178, 183, 187, 191, 195, 200, 204, 208,
//212, 217, 221, 225, 229, 234, 238, 242, 246, 251, 255 };

//// the real HSV rainbow
//void trueHSV(byte LED, int angle)
//{
//    byte red, green, blue;
//
//    if (angle < 60) { red = 255; green = HSVlights[angle]; blue = 0; }
//    else
//        if (angle < 120) { red = HSVlights[120 - angle]; green = 255; blue = 0; }
//        else
//            if (angle < 180) { red = 0, green = 255; blue = HSVlights[angle - 120]; }
//            else
//                if (angle < 240) { red = 0, green = HSVlights[240 - angle]; blue = 255; }
//                else
//                    if (angle < 300) { red = HSVlights[angle - 240], green = 0; blue = 255; }
//                    else
//                    {
//                        red = 255, green = 0; blue = HSVlights[360 - angle];
//                    }
//    setRGBpoint(LED, red, green, blue);
//}
