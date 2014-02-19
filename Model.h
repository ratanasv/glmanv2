#pragma once
#include <map>
#include <memory>
#include <vector>

struct Uniform {
	std::string _name;
	float _minValue;
	float _maxValue;
	float _value;
	Uniform(const std::string& name = "default", float min = 0.0, float max = 1.0, 
		float val = 0.5) {
		_name = name;
		_minValue = min;
		_maxValue = max;
		_value = val;
	}
};

class GLSLProgram;
class VirModel;
class GLTexture;

struct Texture {
	std::string _name;
	std::shared_ptr<GLTexture> _glTexture;
	Texture(const std::string& name, const std::shared_ptr<GLTexture> glTexture) {
		_name = name;
		_glTexture = glTexture;
	}
};


extern float m_xRot;
extern float m_yRot;
extern float m_xMouse;
extern float m_yMouse;
extern float m_zTranslate;
extern std::shared_ptr<GLSLProgram> m_shader;
extern std::map<std::string, Uniform> m_Uniforms;
extern std::shared_ptr<VirModel> m_model;
extern std::vector<Texture> m_textures;