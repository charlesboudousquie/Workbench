#pragma once
#include "GraphicsManipulatorInterface.hpp"

class systemManagerInterface;

class graphicsManipulator : public graphicsManipulatorInterface
{
public:
	graphicsManipulator(systemManagerInterface * p_system_manager);

	// use with setRenderTextureIndex in editor
	// only changes when graphics settings change
	std::vector<std::string> getRenderTextureNames() override;

	// -1 = do not show a texture
	// otherwise index into getRenderTextureNames
	void setRenderTextureIndex(int index) override;

	int getRenderTextureIndex() override;

	// reloads all shaders currently in use
	void reloadShaders() override;


private:
	systemManagerInterface * m_system_manager;
};
