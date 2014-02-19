#pragma once
#include <memory>
#include <cstring>
#include "glew.h"
#include <atomic>
#include <thread>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

using std::shared_ptr;
using std::string;
using std::atomic_uint;

GLenum toGLTexInternalFormat(const int ch);
GLenum toGLTexFormat(const int ch);

class TextureVisitor {
public:
	virtual ~TextureVisitor() {};
	virtual void PreRender(const unsigned int whichTex, GLenum bindSite, 
		const shared_ptr<unsigned> texHandle) = 0;
};


//--------START OF FACTORY----------
class TextureData {

public:
	virtual GLenum getInternalFormat() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual int getDepth() = 0;
	virtual GLenum getFormat() = 0;
	virtual GLenum getType() = 0;
	virtual shared_ptr<const void> get_data() = 0;

	virtual ~TextureData() {};
};

//thread-safe because it is immutable
class Texture2DData: public TextureData {
private:
	shared_ptr<unsigned char> _texels;
	int _width;
	int _height;
	GLenum _internalFormat;
	int _depth;
	GLenum _format;
	GLenum _type;
public:
	explicit Texture2DData(const string& file_name);
private:
	void flip_vertically();
public:
	virtual shared_ptr<const void> get_data();

};

class BinaryTexture2DData : public TextureData {
public:
	explicit BinaryTexture2DData(const string& fileName);

	virtual GLenum getInternalFormat();

	virtual int getWidth();

	virtual int getHeight();

	virtual int getDepth();

	virtual GLenum getFormat();

	virtual GLenum getType();

	virtual shared_ptr<const void> get_data();
private:
	std::shared_ptr<const void> _data;

};

class GLTexture {
public:
	GLTexture(const shared_ptr<TextureData>& factory);
	virtual void send_to_gpu(const shared_ptr<TextureData>& factory);
	virtual void pre_render(const shared_ptr<TextureVisitor>& visitor);
	virtual void post_render();
protected:
	shared_ptr<unsigned> _tex_handle;
	const unsigned _which_tex;
	GLenum _bind_site;
	boost::shared_mutex _mutex;
private:
	static atomic_uint OGLActiveTextureCounter;
};

class GLSLUniformBinder;

class GLSLTextureSamplerBinder : public TextureVisitor {
private:
	const shared_ptr<GLSLUniformBinder> _binder;
	const string _uniformVariable;
public:
	GLSLTextureSamplerBinder(const shared_ptr<GLSLUniformBinder>& binder, 
		const string& uniformVariable);
	virtual void PreRender( const unsigned int whichTex, GLenum bindSite, 
		const shared_ptr<unsigned> texHandle );

};