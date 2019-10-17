#include "GraphicsManipulator.hpp"
#include "GraphicsSystem.hpp"

#include "SystemManagerInterface.hpp"

#include <algorithm>

graphicsManipulator::graphicsManipulator(systemManagerInterface * p_system_manager)
	: m_system_manager{p_system_manager}
{
}

std::vector<std::string> graphicsManipulator::getRenderTextureNames()
{
	return std::vector<std::string>{
		graphicsSystem::textureNames.cbegin(),
		graphicsSystem::textureNames.cend()
	};
}

void graphicsManipulator::setRenderTextureIndex(int index)
{
	auto l_graphics = m_system_manager->getSystem<graphicsSystem>();
	l_graphics->m_textureIndex = std::clamp<int>(index, -1, graphicsSystem::s_texCount);
}

int graphicsManipulator::getRenderTextureIndex()
{
	auto l_graphics = m_system_manager->getSystem<graphicsSystem>();
	return l_graphics->m_textureIndex;
}
