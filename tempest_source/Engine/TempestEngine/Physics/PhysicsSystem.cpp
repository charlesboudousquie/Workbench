/*!***************************************************************************************
\file       PhysicsSystem.cpp
\author     Ryan Booth
\date       3/8/19
\copyright  All content 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
//#include "../../Bullet/BulletWrapper.hpp"
#include "PhysicsSystem.hpp"
#include "../GameObjectFiltering/ComponentTypeSet.hpp"
#include "../Components/RigidBody.hpp"
#include "../SystemManager.hpp"
#include "../GameObjectFiltering/GameObjectFilter.hpp"
#include <Logger.hpp>
#include "../Events/EventSystem.hpp"
#include "../Events/CollisionEvent.hpp"
#include "../SceneManagement/Transform.hpp"
#include <Vector3.hpp>

static bool init = false;

void physicsSystem::onInitialize()
{
	onLevelLoad(nullptr);
}

void physicsSystem::onStartFrame()
{
  if (!init)
  {
    addAllObjectsToBullet();
    init = true;
  }
  else
  {
    updateOurObjectsFromBullet();
  }
}


void physicsSystem::onUpdate()
{
   
}

void physicsSystem::onEndFrame()
{
  if(!bulletWrapper::isRunning())
  {
    return;
  }

  //Update all physics objects that have dirty transforms 
  GameObjectFiltering::componentTypeSet l_pattern3;
  l_pattern3.setType(rigidBody::getType());
  auto l_listRigidBody = getSystemManager()->getGameObjectGatherer()->getList(l_pattern3);

  for (auto& rbObject : l_listRigidBody)
  {
    auto l_rb = rbObject->getComponent<rigidBody>();
    auto l_transform = rbObject->getComponent<transform>();

    if(l_transform->getPhysicsDirtyFlag())
    {
      vector3 l_transformVec = l_transform->getGlobalPosition();
      quaternion l_rotation = l_transform->getGlobalRotation();
      vector3 l_scale = l_transform->getGlobalScale();
      
      //Position, rotation, scale
      bulletWrapper::updateTransform(l_rb->getHandle(), btVector3(l_transformVec.x , l_transformVec.y, l_transformVec.z),
                        btQuaternion(l_rotation.x, l_rotation.y, l_rotation.z, l_rotation.w), btVector3(l_scale.x, l_scale.y, l_scale.z));
    }
  }
}

void physicsSystem::onShutdown()
{
	onLevelUnload(nullptr);
}

void physicsSystem::onLevelLoad(const levelLoadEvent * /*p_event*/)
{
	logger("physicsSystem").debug("onLevelLoad");
	//m_bullet = new bulletWrapper();
	m_globalGravity = vector3(0.0f, -400.0f, 0.0f);
	init = false;
  initializeBullet();
}

void physicsSystem::onLevelUnload(const levelUnloadEvent * /*p_event*/)
{
	logger("physicsSystem").debug("onLevelUnload");

	if(init == true)
  {
    init = false;
    bulletWrapper::destroySimulator();
  }
}

void physicsSystem::setGlobalGravity(const vector3& p_globalGravity)
{
  m_globalGravity = p_globalGravity;
  bulletWrapper::setBTGlobalGravity(btVector3(m_globalGravity.x, m_globalGravity.y, m_globalGravity.z));
}

vector3 physicsSystem::getGlobalGravity()
{
  return m_globalGravity;
}

objID physicsSystem::getObjectRayCast(const vector3& p_direction, collisionLayer p_collidesWith, std::shared_ptr<gameObject> p_gameObject)
{
  if(!bulletWrapper::isRunning())
  {
    return 0;
  }

  btCollisionObject * l_handle = nullptr;

  vector3 l_position = p_gameObject->getComponent<transform>()->getGlobalPosition();

  if (p_gameObject->getComponent<rigidBody>()->getObjectRayCast(l_position, l_position + p_direction, p_collidesWith, l_handle))
  {
    //Get the objID associated with that handle
    systemManagerInterface * l_system = getSystemManager();

    GameObjectFiltering::componentTypeSet l_set;
    l_set.setType(rigidBody::getType());
    auto l_list = l_system->getGameObjectGatherer()->getList(l_set);

    for (auto l_gameObject : l_list)
    {
      if(l_gameObject->getComponent<rigidBody>()->getHandle() == l_handle)
      {
        return l_gameObject->getID();
      }
    }
  }

  return 0;
}

std::vector<std::shared_ptr<gameObject> > physicsSystem::getObjectsWithinRadius(const vector3 & p_point, const float & p_radius)
{
  std::vector<std::shared_ptr<gameObject> > l_objects;

  if(!bulletWrapper::isRunning())
  {
    return l_objects;
  }

  GameObjectFiltering::componentTypeSet l_input_component_pattern;
  l_input_component_pattern.setType(rigidBody::getType());
  std::vector<std::shared_ptr<gameObject>> l_list = getSystemManager()->getGameObjectGatherer()->getList(l_input_component_pattern);

  vector3 my_position = p_point;

  for (auto i_object : l_list)
  {
    vector3 position = i_object->getComponent<transform>()->getPosition();
    float x_distance = abs(my_position.x - position.x);
    float y_distance = abs(my_position.y - position.y);
    float z_distance = abs(my_position.z - position.z);

    if (x_distance == 0 && y_distance == 0 && z_distance == 0)
    {
      continue;
    }
    if (x_distance <= p_radius && y_distance <= p_radius && z_distance <= p_radius)
    {
      l_objects.push_back(i_object);
    }
  }

  return l_objects;
}

void physicsSystem::addPhysicsObjectToWorld(std::shared_ptr<gameObject> p_gameObject)
{
  if(!bulletWrapper::isRunning())
  {
    return;
  }

  auto l_rb = p_gameObject->getComponent<rigidBody>();
  auto l_transform = p_gameObject->getComponent<transform>();

  std::pair<unsigned int, unsigned int> l_collisionLayer = l_rb->getCollisionLayer();

  btVector3 l_localGravity;

  if (l_rb->isUsingGlobalGravity())
  {
    l_localGravity = btVector3(m_globalGravity.x, m_globalGravity.y, m_globalGravity.z);
  }
  else
  {
    vector3 l_tempGravity = l_rb->getLocalGravity();
    l_localGravity = btVector3(l_tempGravity.x, l_tempGravity.y, l_tempGravity.z);
  }

  vector3 l_position = l_transform->getGlobalPosition();
  vector3 l_scale = l_transform->getGlobalScale();
  quaternion l_rotation = l_transform->getGlobalRotation();
  vector3 l_localInertia = l_rb->getLocalInertia();

  // add to the world and save handle
  l_rb->setHandle(bulletWrapper::addPhysicsObject(l_rb->getMass(),
    /* Scale    */                      btVector3(l_scale.x, l_scale.y, l_scale.z),
    /* Position */                      btVector4(l_position.x, l_position.y, l_position.z, 1),
    /* Rotation */                      btQuaternion(l_rotation.x, l_rotation.y, l_rotation.z, l_rotation.w),
    /* Inertia  */                      btVector3(l_localInertia.x, l_localInertia.y, l_localInertia.z),
    /* Gravity  */                      l_localGravity,
    /* Restitution, Friction, Shape#*/  l_rb->getRestitution(), l_rb->getFriction(), l_rb->getDamping(), static_cast<int>(l_rb->getCollisionShape()), static_cast<int>(l_collisionLayer.first), static_cast<int>(l_collisionLayer.second)));
}

void physicsSystem::setSimulationTimeStep(float p_dt)
{
  bulletWrapper::setTimeStep(p_dt);
}

void physicsSystem::processCallback(btPersistentManifold* const& p_manifold, unsigned short p_type)
{
  GameObjectFiltering::componentTypeSet l_rigid_body_pattern;
  l_rigid_body_pattern.setType(rigidBody::getType());
  std::vector<std::shared_ptr<gameObject>> l_list = getSystemManager()->getGameObjectGatherer()->getList(l_rigid_body_pattern);

  auto l_btObject1 = p_manifold->getBody0();
  auto l_btObject2 = p_manifold->getBody1();

  objID l_object1 = 0;
  objID l_object2 = 0;

  //Grab both object id's of the collided objects
  for (auto l_object : l_list)
  {
    auto l_rb = l_object->getComponent<rigidBody>();

    if (l_rb->getHandle() == l_btObject1)
    {
      l_object1 = l_object->getID();
    }
    else if (l_rb->getHandle() == l_btObject2)
    {
      l_object2 = l_object->getID();
    }
  }

  std::vector<vector3> l_collisionPoints(p_manifold->getNumContacts(), vector3());
  std::vector<vector3> l_collisionNormals(p_manifold->getNumContacts(), vector3());

  //Get collision points and normals
  for (int i = 0; i < p_manifold->getNumContacts(); ++i)
  {
    auto l_point = p_manifold->getContactPoint(i).getPositionWorldOnB();
    auto l_normal = p_manifold->getContactPoint(i).m_normalWorldOnB;

    l_collisionPoints[i] = vector3((l_point.getX()), l_point.getY(), l_point.getZ());
    l_collisionNormals[i] = vector3(l_normal.getX(), l_normal.getY(), l_normal.getZ());
  }

  //CollisionStartedCallback
  if(p_type == 0)
  {
    getSystemManager()->getSystem<eventSystem>()->QueueEvent(new collisionEvent("Collision Started", l_object1, l_object2, p_manifold->getNumContacts(), l_collisionPoints, l_collisionNormals));
  }
  //CollisionEndedCallback
  else if(p_type == 1)
  {
    getSystemManager()->getSystem<eventSystem>()->QueueEvent(new collisionEvent("Collision Ended", l_object1, l_object2, p_manifold->getNumContacts(), l_collisionPoints, l_collisionNormals));
  }

}

void physicsSystem::initializeBullet()
{
  // Create the Bullet world
  bulletWrapper::createSimulator(btVector3(m_globalGravity.x, m_globalGravity.y, m_globalGravity.z), getSystemManager());

}

void physicsSystem::addAllObjectsToBullet()
{
  //We have already added all our objects to bullet
  if(init)
  {
    return;
  }

  GameObjectFiltering::componentTypeSet l_pattern3;
  l_pattern3.setType(rigidBody::getType());
  auto l_listRigidBody = getSystemManager()->getGameObjectGatherer()->getList(l_pattern3);

  // add each object with a rigidbody to the world
  for (auto& rbObject : l_listRigidBody)
  {
    auto l_rb = rbObject->getComponent<rigidBody>();
    auto l_transform = rbObject->getComponent<transform>();

    std::pair<unsigned int, unsigned int> l_collisionLayer = l_rb->getCollisionLayer();

    btVector3 l_localGravity;

    if (l_rb->isUsingGlobalGravity())
    {
      l_localGravity = btVector3(m_globalGravity.x, m_globalGravity.y, m_globalGravity.z);
    }
    else
    {
      vector3 l_tempGravity = l_rb->getLocalGravity();
      l_localGravity = btVector3(l_tempGravity.x, l_tempGravity.y, l_tempGravity.z);
    }

    vector3 l_position = l_transform->getGlobalPosition();
    vector3 l_scale = l_transform->getGlobalScale();
    quaternion l_rotation = l_transform->getGlobalRotation();
    vector3 l_localInertia = l_rb->getLocalInertia();

    // add to the world and save handle
    l_rb->setHandle(bulletWrapper::addPhysicsObject(l_rb->getMass(),
      /* Scale    */                      btVector3(l_scale.x, l_scale.y, l_scale.z),
      /* Position */                      btVector4(l_position.x, l_position.y, l_position.z, 1),
      /* Rotation */                      btQuaternion(l_rotation.x, l_rotation.y, l_rotation.z, l_rotation.w),
      /* Inertia  */                      btVector3(l_localInertia.x, l_localInertia.y, l_localInertia.z),
      /* Gravity  */                      l_localGravity,
      /* Restitution, Friction, Shape#*/  l_rb->getRestitution(), l_rb->getFriction(), l_rb->getDamping(), static_cast<int>(l_rb->getCollisionShape()), static_cast<int>(l_collisionLayer.first), static_cast<int>(l_collisionLayer.second)));
  }
}

void physicsSystem::updateOurObjectsFromBullet()
{
  if(!bulletWrapper::isRunning())
  {
    return;
  }

  GameObjectFiltering::componentTypeSet l_rigid_body_pattern;
  l_rigid_body_pattern.setType(rigidBody::getType());
  auto l_listRigidBody = getSystemManager()->getGameObjectGatherer()->getList(l_rigid_body_pattern);

  bulletWrapper::updatePhysicsWorld();

  // Copy bullet's transform over to ours
  for (auto& rbObject : l_listRigidBody)
  {
    auto l_rb = rbObject->getComponent<rigidBody>();
    auto l_transform = rbObject->getComponent<transform>();

    btQuaternion l_rotation;
    btVector4 l_position = bulletWrapper::updatePhysicsObject(l_rb->getHandle(), l_rotation);

    if (rbObject->getParent().lock() == nullptr)
    {
      l_transform->setPosition(vector3(l_position.getX(), l_position.getY(), l_position.getZ()));
      //l_transform->setRotation(quaternion(l_rotation.getX(), l_rotation.getY(), l_rotation.getZ(), l_rotation.getW()));
    }


    l_transform->is_physicsDirty = false;
  }
}
