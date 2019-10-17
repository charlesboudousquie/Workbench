#pragma once
#include <vector>
#include <string>



class graphicsManipulatorInterface
{
public:
	

	virtual std::vector<std::string> getRenderTextureNames() = 0;

	// -1 = do not show a texture
	virtual void setRenderTextureIndex(int index) = 0;
	virtual int getRenderTextureIndex() = 0;

	virtual void reloadShaders() = 0;


};