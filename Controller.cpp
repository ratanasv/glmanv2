#include "Model.h"
#include <string>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "vir_toolbox.hpp"
#include "glslprogram.h"
#include "Controller.h"

using namespace std;

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
	m_shader = new GLSLProgram();
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
