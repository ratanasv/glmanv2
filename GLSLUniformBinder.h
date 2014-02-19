#pragma once

#include <cstring>


class GLSLUniformBinder {
public:
	virtual ~GLSLUniformBinder() {};
	virtual void SetUniform(const std::string& name, int i) = 0;
	virtual void SetUniform(const std::string& name, float f) = 0;
};