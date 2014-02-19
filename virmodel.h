#pragma once
#include <memory>
#include <vector>
#include "vec.h"
#include "objload.h"
#include "Angel.h"

using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using Angel::vec3;

class GLSLAttributeBinder;


class GeometryAbstractFactory{
public:
	virtual ~GeometryAbstractFactory(){};
	virtual shared_ptr<vector<vec3<GLfloat>>> get_vertices()=0;
	virtual shared_ptr<vector<vec3<GLfloat>>> get_normals()=0;
	virtual shared_ptr<vector<vec3<GLfloat>>> get_tex_coord()=0;
	virtual shared_ptr<vector<unsigned>> get_indices()=0;
};

class OBJFactory: public GeometryAbstractFactory {
public:
	OBJFactory(const char* fn);
	virtual shared_ptr<vector<vec3<GLfloat>>> get_vertices();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_normals();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_tex_coord();
	virtual shared_ptr<vector<unsigned>> get_indices();
private:
	void initVertices();
	void initNormals();
	void initTexCoords();
	void initIndices();
	obj::ObjModel _model;
	shared_ptr<vector<vec3<GLfloat>>> vertices;
	shared_ptr<vector<vec3<GLfloat>>> normals;
	shared_ptr<vector<vec3<GLfloat>>> texCoords;
	shared_ptr<vector<unsigned>> indices;
};

class CubeGeometryFactory : public GeometryAbstractFactory {
public:
	CubeGeometryFactory(const float radius = 1.0) : _radius(radius) {};

	virtual shared_ptr<vector<vec3<GLfloat>>> get_vertices();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_normals();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_tex_coord();
	virtual shared_ptr<vector<unsigned>> get_indices();
private:
	const float _radius;
};

class PlaneGeometryFactory : public GeometryAbstractFactory {
public:
	PlaneGeometryFactory(const float radius = 1.0) : _radius(radius) {};

	virtual shared_ptr<vector<vec3<GLfloat>>> get_vertices();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_normals();
	virtual shared_ptr<vector<vec3<GLfloat>>> get_tex_coord();
	virtual shared_ptr<vector<unsigned>> get_indices();
private:
	const float _radius;
};

class GeometryDelegatee {
public:
	virtual ~GeometryDelegatee() {};
protected:
	GeometryDelegatee(const shared_ptr<GeometryAbstractFactory>& factory):
		_factory(factory) {};
	shared_ptr<GeometryAbstractFactory> _factory;
public:
	virtual void send_to_gpu()=0;
	virtual void pre_render()=0;
	virtual void post_render()=0;
	virtual void render()=0;
};

class VAODelegatee : public GeometryDelegatee {
public:
	VAODelegatee(const shared_ptr<GeometryAbstractFactory>& factory,
		const weak_ptr<GLSLAttributeBinder>& glslBinder);
	virtual void send_to_gpu();
	virtual void pre_render();
	virtual void post_render();
	virtual void render();
private:
	weak_ptr<GLSLAttributeBinder> _glslBinder;
	unsigned int _vao;
	unsigned _num_indices;
};

class VAOFreeableDelegatee : public VAODelegatee {
public:
	VAOFreeableDelegatee(const shared_ptr<GeometryAbstractFactory>& factory,
		const shared_ptr<GLSLAttributeBinder>& glslBinder) :
		VAODelegatee(factory, glslBinder) {};
	/**
	 * When push comes to shove, free the host-side memory by releasing
	 * ownership of the current geometry factory.
	 */
	virtual void freeCPUMemory();
};

class VirModel {
public:
	virtual ~VirModel() {};
	VirModel(const shared_ptr<GeometryDelegatee>& delegatee) :
		_delegatee(delegatee) {_delegatee->send_to_gpu();};
	shared_ptr<GeometryDelegatee> _delegatee;
public:
	virtual void render() {
		_delegatee->pre_render();
		_delegatee->render();
		_delegatee->post_render();
	}
};
