#include "Model.h"
#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "vir_toolbox.hpp"
#include "glslprogram.h"
#include "Controller.h"
#include "virmodel.h"
#include "virtex.h"

using namespace std;
using namespace rapidjson;

static rapidjson::Document _doc;

string ReadFileContent(const string& fileName) {
	shared_ptr<FILE> fp = InitFilePointer(fileName, "r");

	fseek(fp.get(), 0L, SEEK_END);
	long size = ftell(fp.get());
	fseek(fp.get(), 0L, SEEK_SET);
	char* buffer = new char[size+1];
	memset(buffer, '\0', size+1);
	fread(buffer,1, size, fp.get());
	buffer[size] = '\0';
	return string(buffer);
}

void initShader(const string& jsonFile) {
	m_shader.reset(new GLSLProgram());
	using namespace rapidjson;
	auto jsonString = ReadFileContent(jsonFile);
	if (!_doc.Parse<0>(jsonString.c_str()).HasParseError()) {
		const Value& shaders = _doc["shaders"];
		const int numShaders = shaders.Size();
		const char* shadersList[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
		for (int i=0; i<numShaders; i++) {
			shadersList[i] = shaders[i].GetString();
		}
		m_shader->Create(shadersList[0], shadersList[1], 
			shadersList[2], shadersList[3], shadersList[4],
			shadersList[5]);
	}


}

void initUniforms() {
	using namespace rapidjson;
	const Value& uniforms = _doc["uniforms"];
	const int numUniforms = uniforms.Size();
	for (int i=0; i<numUniforms; i++) {
		auto name = uniforms[i]["name"].GetString();
		auto min = uniforms[i]["minValue"].GetDouble();
		auto max = uniforms[i]["maxValue"].GetDouble();
		auto value = uniforms[i]["value"].GetDouble();
		m_Uniforms[name] = Uniform(name, min, max, value);
	}
}

void initModels() {
	if (_doc.HasMember("model")) {
		string pathToObj = _doc["model"].GetString();
		shared_ptr<GeometryAbstractFactory> objFactory(new OBJFactory(pathToObj.c_str()));
		shared_ptr<GeometryDelegatee> vao(new VAODelegatee(objFactory, 
			m_shader));
		m_model.reset(new VirModel(vao));
		
	} else { //defaults to cube
		shared_ptr<GeometryAbstractFactory> cubeFactory(new CubeGeometryFactory());
		shared_ptr<GeometryDelegatee> vao(new VAODelegatee(cubeFactory, 
			m_shader));
		m_model.reset(new VirModel(vao));
	}
	
}

void initTextures() {
	if (_doc.HasMember("textures")) {
		const Value& textures = _doc["textures"];
		const int numTextures = textures.Size();
		for (int i=0; i<numTextures; i++) {
			const Value& texture = textures[i];
			shared_ptr<TextureData> factory(new BinaryTexture2DData(
				texture["path"].GetString()));
			shared_ptr<GLTexture> glTexture(new GLTexture(factory));
			m_textures.push_back(Texture(texture["name"].GetString(), glTexture));
		}
	} else { 
		//nothing
	}
}
