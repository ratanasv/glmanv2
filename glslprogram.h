#pragma once
#include <cstdio>
#include <cmath>

#ifdef WIN32
#include <windows.h>
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <map>
#include <stdarg.h>
#include <string>
#include "vec.h"
#include "mat.h"
#include "GLSLUniformBinder.h"
#include "GLSLAttributeBinder.h"
#include "GLSLCameraBinder.h"

using std::string;
using Angel::vec3;
using Angel::mat4;

#define NVIDIA_SHADER_BINARY	0x00008e21		// nvidia binary enum

inline int GetOSU( int flag )
{
	int i;
	glGetIntegerv( flag, &i );
	return i;
}


void	CheckGlErrors( const char* );



class GLSLProgram : public GLSLAttributeBinder, public GLSLCameraBinder, 
	public GLSLUniformBinder
{
  private:
	std::map<const string, int>	AttributeLocs;
	char *			Cfile;
	unsigned int		Cshader;
	char *			Ffile;
	unsigned int		Fshader;
	char *			Gfile;
	GLuint			Gshader;
	bool			IncludeGstap;
	GLenum			InputTopology;
	GLenum			OutputTopology;
	GLuint			Program;
	char *			TCfile;
	GLuint			TCshader;
	char *			TEfile;
	GLuint			TEshader;
	std::map<const string, int>	UniformLocs;
	bool			Valid;
	char *			Vfile;
	GLuint			Vshader;
	bool			Verbose;

	static int		CurrentProgram;

	mat4 _mvMatrix;
	mat4 _projMatrix;

	void	AttachShader( GLuint );
	bool	CanDoBinaryFiles;
	bool	CanDoComputeShaders;
	bool	CanDoFragmentShaders;
	bool	CanDoGeometryShaders;
	bool	CanDoTessControlShaders;
	bool	CanDoTessEvaluationShaders;
	bool	CanDoVertexShaders;
	int	CompileShader( GLuint );
	bool	CreateHelper( const char *, ... );
	
	int	GetUniformLocation(const string& name);


  public:
		GLSLProgram( );
	int	GetAttributeLocation( const string& );
	bool	Create( const char*, const char* = NULL, const char* = NULL, const char* = NULL, const char* = NULL, const char* = NULL );
	void	DispatchCompute( GLuint, GLuint = 1, GLuint = 1 );
	bool	IsExtensionSupported( const char * );
	bool	IsNotValid( );
	bool	IsValid( );
	void	LoadBinaryFile( char * );
	void	LoadProgramBinary( const char *, GLenum );
	void	SaveBinaryFile( char * );
	void	SaveProgramBinary( const char *, GLenum * );
	void	SetAttribute( char *, int );
	void	SetAttribute( char *, float );
	void	SetAttribute( char *, float, float, float );
	void	SetAttribute( char *, float[3] );
#ifdef VEC3_H
	void	SetAttribute( char *, Vec3& );
#endif
#ifdef VERTEX_ARRAY_H
	void	SetAttribute( char *, VertexArray&, GLenum );
#endif
#ifdef VERTEX_BUFFER_OBJECT_H
	void	SetAttribute( char *, VertexBufferObject&, GLenum );



#endif
	void	EnableVertexAttribute(const string& name, const int vecLength = 4); 
	void	SetGstap( bool );
	void	SetInputTopology( GLenum );
	void	SetOutputTopology( GLenum );
	void	SetUniform(const string& name, int );
	void	SetUniform(const string& name, float );
	void	SetUniform(const string& name, float, float, float );
	void	SetUniform(const string& name, float[3] );
	void	SetVerbose( bool );
	void	Use( );
	void	Use( GLuint );
	void	UseFixedFunction( );

	// inherit from GLSLAttributeBinder
	virtual void EnablePositionAttribute( const int vecLength = 4 );
	virtual void EnableNormalAttribute( const int vecLength = 4 );
	virtual void EnableColorAttribute( const int vecLength = 4 );
	virtual void EnableTexCoordAttribute( const int vecLength = 4 );

	// inherit from GLSLCameraBinder
	virtual void LookAt( const vec3<>& eye, const vec3<>& at, const vec3<>& up );
	virtual void Ortho( float left, float right, float bottom, float top, 
		float zNear, float zFar );
	virtual void Perspective( float fovy, float ratio, float zNear, float zFar );
	virtual void Rotate( float x, float y, float z );
	virtual void Translate( float x, float y, float z );
	virtual void Scale( float x, float y, float z );
	virtual void ClearProjection();
	virtual void ClearModelView();

private:
	void SendMVMatrix();
	void SendProjMatrix();
};

