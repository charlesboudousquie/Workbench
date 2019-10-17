
#include "EngineRunner.hpp"
#include <Engine.hpp>
#include <ComponentRepository.hpp>

engineRunner::engineRunner(externalWindowInterface * p_window_ptr, renderContextInterface * p_render_context_ptr, inputSourceInterface * p_input_source_ptr)
    : m_component_repo{new componentRepository()},
    m_engine{new engine(p_window_ptr, p_render_context_ptr, p_input_source_ptr, m_component_repo)}
{ }
engineRunner::~engineRunner()
{
    delete m_engine;
    delete m_component_repo;
}

void engineRunner::initialize(configuration p_config)
{
    m_engine->initialize(p_config);
}
void engineRunner::update()
{
    m_engine->update();
}
void engineRunner::render()
{
    m_engine->render();
}
void engineRunner::shutdown()
{
    m_engine->shutdown();
}
