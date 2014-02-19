#include "Model.h"
#include "glslprogram.h"
#include <memory>
#include "virmodel.h"
#include "virtex.h"


float m_xRot = 0.0;
float m_yRot = 0.0;
float m_xMouse = 0.0;
float m_yMouse = 0.0;
float m_zTranslate = 0.0;

std::shared_ptr<GLSLProgram> m_shader;
std::map<std::string, Uniform> m_Uniforms;
std::shared_ptr<VirModel> m_model;
std::vector<Texture> m_textures;
