#pragma once

#include <IComponentRepository.hpp>
#include <unordered_map>
#include <vector>
/*!
track active system doing allocation too
overloads -> operator
private constructors, though a duplicate function
*/

// TODO(coseo) have a list of componenst who have not had their game object set yet - garbage collect them 
// if not set by cleanup time 
class componentRepository : public icomponentRepository
{
public:
	virtual ~componentRepository() override;
private:
	blockHandle* createComponentInternal(unsigned componentID,
										 size_t size,
										 const char * file,
										 const char * func,
										 int line) override;
	void freeComponentInternal(
		component* component,
		blockHandle* handle,
		const char * file,
		const char * func,
		int line) override;
	virtual void returnBlockHandle(blockHandle* handle) override;

};
