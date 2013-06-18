#pragma once
#include "Manager.h"
#include "Shader.h"

class ShaderManager: public Manager<Shader>
{
	ShaderManager(){}
public:
	static ShaderManager& getInstance() 
	{
		static ShaderManager instance;
		return instance;
	}
};