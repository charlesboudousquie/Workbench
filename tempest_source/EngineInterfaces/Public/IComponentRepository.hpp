#pragma once
#include <cstddef>

class component;
class componentType;
#define STRINGIFY(x) #x

#define ALLOCATE(repositoryPtr, type) \
	repositoryPtr->createComponent<type>(STRINGIFY(__FILE__), STRINGIFY(__func__), __LINE__)
#define DEALLOCATE(repositoryPtr, handle) \
	repositoryPtr->freeComponent(handle, STRINGIFY(__FILE__), STRINGIFY(__func__), __LINE__)
	


// could probably remove page pointer?
struct blockHandle
{
	//!< pointer to internal data block
	void* data = 0;
	//!< count of references to this handler - clean up when it reaches 0
	int refCount = 0;
	//!< page that holds our block 
	void* page = 0; 
};

class icomponentRepository;

// cleanup implementation is in ComponentRepository.cpp
// TODO(coseo) remove owner handle, we don't use it 
class baseComponentHandle 
{
public:
	//!< default constructor is null
	baseComponentHandle() {
		bHandle = nullptr;
		owner = nullptr;
		offset = 0;
	}
	//!< publicly visible constructors to build component handles
	baseComponentHandle(blockHandle* handle_,
		icomponentRepository* owner_) : owner(owner_),
		bHandle(handle_),
		offset(0)
	{
		if (bHandle)
		{
			++bHandle->refCount;
		}
	}
	baseComponentHandle(const baseComponentHandle& other) : owner(other.owner),
		bHandle(other.bHandle),
		offset(other.offset)
	{
		if (bHandle) {
			++bHandle->refCount;
		}
	}


	// Doesn't really need to be virtual. have no extra vars 
	~baseComponentHandle()
	{
		cleanup();
	};
	component* operator->()
	{
		return getBaseComponent();
	}

	// doesn't need to be virtual because all deriving classes have no extra vars 
	bool operator==(const baseComponentHandle& other) 
	{
		if (owner == other.owner && bHandle == other.bHandle) 
		{
			return true;
		}
		return false;
	}

	// doesn't need to be virtual because deriving classes have no extra vars 
	baseComponentHandle& operator=(const baseComponentHandle& other) 
	{
		cleanup();
		owner = other.owner;
		bHandle = other.bHandle;
		if (bHandle)
			++bHandle->refCount;
		offset = other.offset;
		return *this;
	}

	bool isNullptr()
	{
		return (bHandle == nullptr || bHandle->data == nullptr);
	}
	operator bool() const 
	{
		return (bHandle != nullptr && bHandle->data != nullptr);
	}
protected:
	// repository that owns us 
	icomponentRepository* owner;
	blockHandle* bHandle;
	size_t offset = 0;

	// this is necessary to allow the templated inheritors to call the base class's
	// protected function on their templated friends 
	static component* getBaseComponentFromOtherStatic(const baseComponentHandle& other) {
		return other.getBaseComponent();
	}
	static icomponentRepository* getOwnerFromOtherStatic(const baseComponentHandle& other) {
		return other.owner;
	}
	static blockHandle* getBhandleFromOtherStatic(const baseComponentHandle& other) {
		return other.bHandle;
	}
	virtual component* getBaseComponent() const = 0;
	void cleanup();
	friend class icomponentRepository;
};

template<typename T>
class componentHandle : public baseComponentHandle
{
public:
	//!< default constructor is null
	componentHandle() {}
	//!< publicly visible constructors to build component handles
	componentHandle(blockHandle* handle_,
					icomponentRepository* owner_) : baseComponentHandle(handle_, owner_)
	{}

	static componentHandle<T> castHandle(const baseComponentHandle& handle)
	{
		// component pointer is a pointer to the type that they want 

		// case: component pointer does not point to null 
		componentHandle<T> result;
		component* ptr = result.getBaseComponentFromOther(handle);
		result.owner = result.getOwnerFromOther(handle);
		result.bHandle = result.getHandleFromOther(handle);
		if (ptr)
		{
			T* cast_result = dynamic_cast<T*>(ptr);
			if (cast_result != nullptr)
			{
				result.offset = reinterpret_cast<char*>(cast_result) -
					reinterpret_cast<char*>(result.bHandle->data);
				++result.bHandle->refCount;
				return result;
			}
		}
		// if it was at all malformed, throw out the data and return no offset 
		result.offset = 0;
		result.bHandle = nullptr;
		return result;
	}
	
	componentHandle(const componentHandle<T>& other) : baseComponentHandle(other)
	{}
	
	// TODO(coseo) is this slow?
	T* operator->() 
	{
		return reinterpret_cast<T*>(reinterpret_cast<char*>(bHandle->data) + offset);
	}
private:
	friend class icomponentRepository;
	virtual component* getBaseComponent() const override 
	{
		if (bHandle)
		{
			return static_cast<component*>(
				reinterpret_cast<T*>(reinterpret_cast<char *>(bHandle->data) + offset));
		}
		return nullptr;
	}
	// These functions are unfortunately necessary in oder to implement the casting 
	blockHandle* getHandleFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getBhandleFromOtherStatic(other);
	}
	icomponentRepository* getOwnerFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getOwnerFromOtherStatic(other);
	}
	component* getBaseComponentFromOther(const baseComponentHandle& other)
	{
		return baseComponentHandle::getBaseComponentFromOtherStatic(other);
	}

	template <typename U>
	friend class componentHandle;
};

class icomponentRepository
{
public:
	template<typename T>
	componentHandle<T> createComponent(const char * file,
									   const char * func,
							   		   int line) 
	{
		// TODO(coseo) integrate with CRTP
		blockHandle* raw_ptr = createComponentInternal(-1, sizeof(T), file, func, line);
		// Placement new to format memory
		raw_ptr->data = reinterpret_cast<void*>(new (raw_ptr->data) T());
		componentHandle<T> result;
		result.bHandle = raw_ptr;
		raw_ptr->refCount++;
		result.owner = this;
		return result;
	}
	virtual ~icomponentRepository() {}

	void freeComponent(baseComponentHandle* handle,
		const char * file,
		const char * func,
		int line)
	{
		freeComponentInternal(handle->getBaseComponent(), handle->bHandle, file, func, line);
		delete handle;
	}
protected:
	virtual void freeComponentInternal(component* component,
		blockHandle* handle,
		const char * file,
		const char * func,
		int line) = 0;
	virtual blockHandle* createComponentInternal(unsigned componentID,
												 size_t size,
												 const char * file,
												 const char * func,
												 int line) = 0;
	virtual void returnBlockHandle(blockHandle* handle) = 0;
	template <typename U>
	friend class componentHandle;
	friend class baseComponentHandle;
};

