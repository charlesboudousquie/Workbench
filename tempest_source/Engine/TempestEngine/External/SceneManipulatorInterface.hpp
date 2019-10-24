/*!***************************************************************************************
\file       SceneManipulatorInterface.hpp
\author     Aaron Damyen
\date       7/31/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
*****************************************************************************************/
#pragma once

#include <Reflection.hpp>
#include <DataTypes.hpp>
#include <vector>
#include <string>
#include <memory>

class scene;
class space;
class gameObject;

class sceneManipulatorInterface
{
public:
    virtual objID getParentID(objID p_object_id) = 0;
    virtual objID getSpaceIDForObject(objID p_object_id) = 0;
    virtual objID getSpaceIDFromName(std::string name) = 0;

	virtual std::vector<objID> getSceneIDs() = 0;
    virtual std::string getSceneName(objID p_scene_id) = 0;
    virtual std::vector<objID> getSpaceIDsForScene(objID p_scene_id) = 0;
    virtual std::string getSpaceName(objID p_space_id) = 0;
	virtual std::vector<objID> getObjectIDsForSpace(objID p_space_id) = 0;
	virtual std::vector<objID> getTopObjectIDsForSpace(objID p_space_id) = 0;
	virtual std::vector<objID> getChildObjectIDsForObject(objID p_object_id) = 0;
	virtual std::string getObjectName(objID p_object_id) = 0;
	virtual typeRT getTypeRT(objID p_object_id) = 0;
	virtual void applyTypeRT(objID p_id, typeRT & p_type) = 0;

  virtual std::shared_ptr<scene> addEmptyScene() = 0;
  virtual std::shared_ptr<space> addEmptySpace() = 0;
  virtual std::shared_ptr<space> addEmptySpace(objID p_scene_id) = 0;
	virtual objID addEmptySpace(const std::string& p_name) = 0;

	virtual std::shared_ptr<gameObject> addEmptyGameObject() = 0;
	virtual std::shared_ptr<gameObject> addEmptyGameObject(objID p_parent_id) = 0;
	virtual objID addEmptyGameObject(objID p_parent_id, const std::string& p_name) = 0;
	virtual void addRenderedGameObject(float p_x, float p_y, float p_z, std::string p_object_name, std::string p_texture) = 0;

	virtual void addGameObjectComponent(objID p_object_id, const std::string & p_component_type) = 0;
	virtual void removeGameObjectComponent(objID p_object_id, const std::string & p_component_type) = 0;

	virtual void readObjectTransform(objID p_object_id, float * p_transform_matrix) = 0;
	virtual void writeObjectTransform(objID p_object_id, float * p_transform_matrix) = 0;
	virtual void createEditorCamera() = 0;
	virtual void removeEditorCamera() = 0;

  virtual void setButtonNeighbor(objID p_objectID, objID p_neighborID, int p_neighbor) = 0;
  virtual objID removeButtonNeighbor(objID p_objectID, int p_neighbor) = 0;
  virtual objID getObjectID(std::string p_objectName) = 0;

  virtual void setCollisionLayer(objID p_object_id, unsigned int p_data, unsigned int p_type) = 0;

  virtual void removeGameObject(objID p_objectID) = 0;

  virtual void addParentToGameObject(objID p_parent_id, objID p_child_id) = 0;
  virtual void removeParent(objID p_child_id) = 0;

  virtual void setGameObjectName(objID p_object_id, const std::string & p_name) = 0;
  virtual void setSceneName(objID p_scene_id, const std::string & p_name) = 0;
  virtual void setSpaceName(objID p_space_id, const std::string & p_name) = 0;

  virtual void deleteSpace(objID p_space_id) = 0;
	virtual void deleteSpace(const std::string& p_space_name) = 0;

  virtual void setButtonMaterial(objID p_button_id, const std::string & p_name) = 0;

  virtual void moveObjectToSpace(objID p_object_id, const std::string & p_space) = 0;
  virtual void moveObjectToSpace(objID p_object_id, objID p_space_id) = 0;

  virtual void dynamicWaypointGraphCreatePath() = 0;
  virtual void dynamicWaypointGraphCreateSetPaths() = 0;
  virtual void dynamicWaypointGraphStitchPaths() = 0;
  virtual void dynamicWaypointGraphClear() = 0;
  virtual void dynamicWaypointGraphDeleteRandomNodeSet() = 0;
};

