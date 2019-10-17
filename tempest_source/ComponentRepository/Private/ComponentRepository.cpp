#include "../Public/ComponentRepository.hpp"
#include "Component.hpp"
componentRepository::~componentRepository() 
{
	// TODO(coseo) clean up memory management here
}
blockHandle* componentRepository::createComponentInternal(unsigned componentID,
														  size_t size,
														  const char * file,
														  const char * func,
														  int line)
{
	// TODO(coseo) enable actual memory management, debug logging
	void* new_component = malloc(size);
	blockHandle* handle = new blockHandle;
	handle->data = new_component;
	handle->page = 0;
	handle->refCount = 0;
	return handle;
}

void componentRepository::freeComponentInternal(
	component* comp,
	blockHandle* handle,
	const char * file,
	const char * func,
	int line)
{
	// Gee I sure hope you wrote a virtual destructor
	comp->~component();
	// TODO(coseo) enable actual memory management, debug logging
	free(handle->data);
	handle->data = nullptr;
	handle->page = nullptr;
}

void componentRepository::returnBlockHandle(blockHandle* handle)
{
	// TODO(coseo) manage memory 
	delete handle;
}

void baseComponentHandle::cleanup()
{
	if (bHandle != nullptr && owner != nullptr)
	{
		if (--bHandle->refCount <= 0)
		{
			// clean up memory internally
			owner->returnBlockHandle(bHandle);
		}
	}
}