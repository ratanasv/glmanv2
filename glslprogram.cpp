#include "glslprogram.h"
#include "mat.h"
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

using Angel::vec4;

struct GLshadertype
{
	char *extension;
	GLenum name;
}
ShaderTypes [ ] =
{
	{ ".comp",   GL_COMPUTE_SHADER },
	{ ".vert", GL_VERTEX_SHADER },
	{ ".vs",   GL_VERTEX_SHADER },
	{ ".frag", GL_FRAGMENT_SHADER },
	{ ".fs",   GL_FRAGMENT_SHADER },
	{ ".geom", GL_GEOMETRY_SHADER },
	{ ".gs",   GL_GEOMETRY_SHADER },
	{ ".tcs",  GL_TESS_CONTROL_SHADER },
	{ ".tes",  GL_TESS_EVALUATION_SHADER },
};

struct GLbinarytype
{
	char *extension;
	GLenum format;
}
BinaryTypes [ ] =
{
	{ ".nvb",    NVIDIA_SHADER_BINARY },
};

extern char *Gstap;		// set later

static const string POSITION_ATTRIBUTE_VAR("vPosition");
static const string NORMAL_ATTRIBUTE_VAR("vNormal");
static const string COLOR_ATTRIBUTE_VAR("vColor");
static const string TEXCOORD_ATTRIBUTE_VAR("vTexCoord");
static const string MODELVIEW_MATRIX_VAR("uModelViewMatrix");
static const string PROJECTION_MATRIX_VAR("uProjectionMatrix");

static const char* GetExtension( const char *file ) {
	int n = (int)strlen(file) - 1;	// index of last non-null character

	// look for a '.':

	do
	{
		if( file[n] == '.' )
			return &file[n];	// the extension includes the '.'
		n--;
	} while( n >= 0 );

	// never found a '.':

	return NULL;
}


GLSLProgram::GLSLProgram( )
{
	Verbose = false;
	InputTopology  = GL_TRIANGLES;
	OutputTopology = GL_TRIANGLE_STRIP;

	CanDoComputeShaders      = IsExtensionSupported( "GL_ARB_compute_shader" );
	CanDoVertexShaders      = IsExtensionSupported( "GL_ARB_vertex_shader" );
	CanDoTessControlShaders = IsExtensionSupported( "GL_ARB_tessellation_shader" );
	CanDoTessEvaluationShaders = CanDoTessControlShaders;
	CanDoGeometryShaders    = IsExtensionSupported( "GL_EXT_geometry_shader4" );
	CanDoFragmentShaders    = IsExtensionSupported( "GL_ARB_fragment_shader" );
	CanDoBinaryFiles        = IsExtensionSupported( "GL_ARB_get_program_binary" );

	fprintf( stderr, "Can do: " );
	if( CanDoComputeShaders )		fprintf( stderr, "compute shaders, " );
	if( CanDoVertexShaders )		fprintf( stderr, "vertex shaders, " );
	if( CanDoTessControlShaders )		fprintf( stderr, "tess control shaders, " );
	if( CanDoTessEvaluationShaders )	fprintf( stderr, "tess evaluation shaders, " );
	if( CanDoGeometryShaders )		fprintf( stderr, "geometry shaders, " );
	if( CanDoFragmentShaders )		fprintf( stderr, "fragment shaders, " );
	if( CanDoBinaryFiles )			fprintf( stderr, "binary shader files " );
	fprintf( stderr, "\n" );
}


// this is what is exposed to the user
// file1 - file5 are defaulted as NULL if not given
// CreateHelper is a varargs procedure, so must end in a NULL argument,
//	which I know to supply but I'm worried users won't

bool
GLSLProgram::Create( const char* file0, const char* file1, const char* file2, const char* file3, const char*  file4, const char* file5 )
{
	return CreateHelper( file0, file1, file2, file3, file4, file5, NULL );
}


// this is the varargs version of the Create method

bool
GLSLProgram::CreateHelper( const char *file0, ... )
{
	GLsizei n = 0;
	GLchar *buf;
	Valid = true;

	IncludeGstap = false;
	Cshader = Vshader = TCshader = TEshader = Gshader = Fshader = 0;
	Program = 0;
	AttributeLocs.clear();
	UniformLocs.clear();

	if( Program == 0 )
	{
		Program = glCreateProgram( );
		CheckGlErrors( "glCreateProgram" );
	}

	va_list args;
	va_start( args, file0 );

	// This is a little dicey
	// There is no way, using var args, to know how many arguments were passed
	// I am depending on the caller passing in a NULL as the final argument.
	// If they don't, bad things will happen.

	const char *file = file0;
	int type;
	while( file != NULL )
	{
		int maxBinaryTypes = sizeof(BinaryTypes) / sizeof(struct GLbinarytype);
		type = -1;
		const char* extension = GetExtension( file );
		// fprintf( stderr, "File = '%s', extension = '%s'\n", file, extension );

		for( int i = 0; i < maxBinaryTypes; i++ )
		{
			if( strcmp( extension, BinaryTypes[i].extension ) == 0 )
			{
				// fprintf( stderr, "Legal extension = '%s'\n", extension );
				LoadProgramBinary( file, BinaryTypes[i].format );
				break;
			}
		}

		int maxShaderTypes = sizeof(ShaderTypes) / sizeof(struct GLshadertype);
		for( int i = 0; i < maxShaderTypes; i++ )
		{
			if( strcmp( extension, ShaderTypes[i].extension ) == 0 )
			{
				// fprintf( stderr, "Legal extension = '%s'\n", extension );
				type = i;
				break;
			}
		}

		if( type < 0 )
		{
			fprintf( stderr, "Unknown filename extension: '%s'\n", extension );
			fprintf( stderr, "Legal Extensions are: " );
			for( int i = 0; i < maxBinaryTypes; i++ )
			{
				if( i != 0 )	fprintf( stderr, " , " );
				fprintf( stderr, "%s", BinaryTypes[i].extension );
			}
			fprintf( stderr, "\n" );
			for( int i = 0; i < maxShaderTypes; i++ )
			{
				if( i != 0 )	fprintf( stderr, " , " );
				fprintf( stderr, "%s", ShaderTypes[i].extension );
			}
			fprintf( stderr, "\n" );
			Valid = false;
			goto cont;
		}

		GLuint shader;
		switch( ShaderTypes[type].name )
		{
			case GL_COMPUTE_SHADER:
				if( ! CanDoComputeShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle compute shaders\n" );
					Valid = false;
					goto cont;
				}
				shader = glCreateShader( GL_COMPUTE_SHADER );
				break;

			case GL_VERTEX_SHADER:
				if( ! CanDoVertexShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle vertex shaders\n" );
					Valid = false;
					goto cont;
				}
				shader = glCreateShader( GL_VERTEX_SHADER );
				break;

			case GL_TESS_CONTROL_SHADER:
				if( ! CanDoTessControlShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle tessellation control shaders\n" );
					Valid = false;
					goto cont;
				}
				shader = glCreateShader( GL_TESS_CONTROL_SHADER );
				break;

			case GL_TESS_EVALUATION_SHADER:
				if( ! CanDoTessEvaluationShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle tessellation evaluation shaders\n" );
					Valid = false;
					goto cont;
				}
				shader = glCreateShader( GL_TESS_EVALUATION_SHADER );
				break;

			case GL_GEOMETRY_SHADER:
				if( ! CanDoGeometryShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle geometry shaders\n" );
					Valid = false;
					goto cont;
				}
				//glProgramParameteriEXT( Program, GL_GEOMETRY_INPUT_TYPE_EXT,  InputTopology );
				//glProgramParameteriEXT( Program, GL_GEOMETRY_OUTPUT_TYPE_EXT, OutputTopology );
				//glProgramParameteriEXT( Program, GL_GEOMETRY_VERTICES_OUT_EXT, 1024 );
				shader = glCreateShader( GL_GEOMETRY_SHADER );
				break;

			case GL_FRAGMENT_SHADER:
				if( ! CanDoFragmentShaders )
				{
					fprintf( stderr, "Warning: this system cannot handle fragment shaders\n" );
					Valid = false;
					goto cont;
				}
				shader = glCreateShader( GL_FRAGMENT_SHADER );
				break;
		}


		// read the shader source into a buffer:

		FILE * in;
		int length;
		FILE * logfile;

		in = fopen( file, "rb" );
		if( in == NULL )
		{
			fprintf( stderr, "Cannot open shader file '%s'\n", file );
			Valid = false;
			goto cont;
		}

		fseek( in, 0, SEEK_END );
		length = ftell( in );
		fseek( in, 0, SEEK_SET );		// rewind

		buf = new GLchar[length+1];
		fread( buf, sizeof(GLchar), length, in );
		buf[length] = '\0';
		fclose( in ) ;


		// Tell GL about the source:

		glShaderSource( shader, 1, (const GLchar **)&buf, NULL );
		delete [ ] buf;
		CheckGlErrors( "Shader Source" );

		// compile:

		glCompileShader( shader );
		GLint infoLogLen;
		GLint compileStatus;
		CheckGlErrors( "CompileShader:" );
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );

		if( compileStatus == 0 )
		{
			fprintf( stderr, "Shader '%s' did not compile.\n", file );
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLogLen );
			if( infoLogLen > 0 )
			{
				GLchar *infoLog = new GLchar[infoLogLen+1];
				glGetShaderInfoLog( shader, infoLogLen, NULL, infoLog);
				infoLog[infoLogLen] = '\0';
				logfile = fopen( "glsllog.txt", "w");
				if( logfile != NULL )
				{
					fprintf( logfile, "\n%s\n", infoLog );
					fclose( logfile );
				}
				fprintf( stderr, "\n%s\n", infoLog );
				delete [ ] infoLog;
			}
			glDeleteShader( shader );
			Valid = false;
			goto cont;
		}
		else
		{
			if( Verbose )
				fprintf( stderr, "Shader '%s' compiled.\n", file );

			glAttachShader( this->Program, shader );
		}


cont:
		// go to the next file:

		file = va_arg( args, char * );
	}

	va_end( args );

	// link the entire shader program:

	glLinkProgram( Program );
	CheckGlErrors( "Link Shader 1");

	GLchar* infoLog;
	GLint infoLogLen;
	GLint linkStatus;
	glGetProgramiv( this->Program, GL_LINK_STATUS, &linkStatus );
	CheckGlErrors("Link Shader 2");

	if( linkStatus == 0 )
	{
		glGetProgramiv( this->Program, GL_INFO_LOG_LENGTH, &infoLogLen );
		fprintf( stderr, "Failed to link program -- Info Log Length = %d\n", infoLogLen );
		if( infoLogLen > 0 )
		{
			infoLog = new GLchar[infoLogLen+1];
			glGetProgramInfoLog( this->Program, infoLogLen, NULL, infoLog );
			infoLog[infoLogLen] = '\0';
			fprintf( stderr, "Info Log:\n%s\n", infoLog );
			delete [ ] infoLog;

		}
		glDeleteProgram( Program );
		Valid = false;
	}
	else
	{
		if( Verbose )
			fprintf( stderr, "Shader Program linked.\n" );
		// validate the program:

		GLint status;
		glValidateProgram( Program );
		glGetProgramiv( Program, GL_VALIDATE_STATUS, &status );
		if( status == GL_FALSE )
		{
			fprintf( stderr, "Program is invalid.\n" );
			Valid = false;
		}
		else
		{
			if( Verbose )
				fprintf( stderr, "Shader Program validated.\n" );
		}
	}

	return Valid;
}


void
GLSLProgram::DispatchCompute( GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z )
{
	Use( );
	glDispatchCompute( num_groups_x, num_groups_y, num_groups_z );
}


bool
GLSLProgram::IsValid( )
{
	return Valid;
}


bool
GLSLProgram::IsNotValid( )
{
	return ! Valid;
}


void
GLSLProgram::SetVerbose( bool v )
{
	Verbose = v;
}


void
GLSLProgram::Use( )
{
	Use( this->Program );
};


void
GLSLProgram::Use( GLuint p )
{
	if( p != CurrentProgram )
	{
		glUseProgram( p );
		CurrentProgram = p;
	}
};


void
GLSLProgram::UseFixedFunction( )
{
	this->Use( 0 );
};


int
GLSLProgram::GetAttributeLocation( const string& name ) {
	auto pos = AttributeLocs.find( name );
	if (pos == AttributeLocs.end()) {
		AttributeLocs[name] = glGetAttribLocation( this->Program, name.c_str() );
	}

	return AttributeLocs[name];
};


#ifdef NOT_SUPPORTED
void
GLSLProgram::SetAttribute( char* name, int val )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib1i( loc, val );
	}
};
#endif


void
GLSLProgram::SetAttribute( char* name, float val )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib1f( loc, val );
	}
};


void
GLSLProgram::SetAttribute( char* name, float val0, float val1, float val2 )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib3f( loc, val0, val1, val2 );
	}
};


void
GLSLProgram::SetAttribute( char* name, float vals[3] )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glVertexAttrib3fv( loc, vals );
	}
};

void GLSLProgram::SendMVMatrix() {
	int loc = GetUniformLocation(MODELVIEW_MATRIX_VAR.c_str());
	if (loc >= 0) {
		this->Use();
		glUniformMatrix4fv(loc, 1, GL_TRUE, _mvMatrix);
	}
}

void GLSLProgram::SendProjMatrix() {
	int loc = GetUniformLocation(PROJECTION_MATRIX_VAR.c_str());
	if (loc >= 0) {
		this->Use();
		glUniformMatrix4fv(loc, 1, GL_TRUE, _projMatrix);
	}
}


#ifdef VEC3_H
void
GLSLProgram::SetAttribute( char* name, Vec3& v );
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		float vec[3];
		v.GetVec3( vec );
		this->Use();
		glVertexAttrib3fv( loc, 3, vec );
	}
};
#endif


#ifdef VERTEX_BUFFER_OBJECT_H
void
GLSLProgram::SetAttribute( char *name, VertexBufferObject& vb, GLenum which )
{
	int loc;
	if( ( loc = GetAttributeLocation( name ) )  >= 0 )
	{
		this->Use();
		glEnableVertexAttribArray( loc );
		switch( which )
		{
			case GL_VERTEX:
				glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(?) );
				break;

			case GL_NORMAL:
				glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(?) );
				break;

			case GL_COLOR:
				glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(?) );
				break;
	}
};
#endif




int
GLSLProgram::GetUniformLocation(const string& name)
{
	auto pos = UniformLocs.find( name );
	if( pos == UniformLocs.end() ) {
		UniformLocs[name] = glGetUniformLocation( this->Program, name.c_str() );
	}

	return UniformLocs[name];
};


void
GLSLProgram::SetUniform(const string& name, int val )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform1i( loc, val );
	}
};


void
GLSLProgram::SetUniform(const string& name, float val )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform1f( loc, val );
	}
};


void
GLSLProgram::SetUniform(const string& name, float val0, float val1, float val2 )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform3f( loc, val0, val1, val2 );
	}
};


void
GLSLProgram::SetUniform(const string& name, float vals[3] )
{
	int loc;
	if( ( loc = GetUniformLocation( name ) )  >= 0 )
	{
		this->Use();
		glUniform3fv( loc, 3, vals );
	}
};


void
GLSLProgram::SetInputTopology( GLenum t )
{
	if( t != GL_POINTS  && t != GL_LINES  &&  t != GL_LINES_ADJACENCY_EXT  &&  t != GL_TRIANGLES  &&  t != GL_TRIANGLES_ADJACENCY_EXT )
	{
		fprintf( stderr, "Warning: You have not specified a supported Input Topology\n" );
	}
	InputTopology = t;
}


void
GLSLProgram::SetOutputTopology( GLenum t )
{
	if( t != GL_POINTS  && t != GL_LINE_STRIP  &&  t != GL_TRIANGLE_STRIP )
	{
		fprintf( stderr, "Warning: You have not specified a supported Onput Topology\n" );
	}
	OutputTopology = t;
}




bool
GLSLProgram::IsExtensionSupported( const char *extension )
{
	// see if the extension is bogus:

	if( extension == NULL  ||  extension[0] == '\0' )
		return false;

	GLubyte *where = (GLubyte *) strchr( extension, ' ' );
	if( where != 0 )
		return false;

	// get the full list of extensions:

	const GLubyte *extensions = glGetString( GL_EXTENSIONS );

	for( const GLubyte *start = extensions; ; )
	{
		where = (GLubyte *) strstr( (const char *) start, extension );
		if( where == 0 )
			return false;

		GLubyte *terminator = where + strlen(extension);

		if( where == start  ||  *(where - 1) == '\n'  ||  *(where - 1) == ' ' )
			if( *terminator == ' '  ||  *terminator == '\n'  ||  *terminator == '\0' )
				return true;
		start = terminator;
	}
	return false;
}


int GLSLProgram::CurrentProgram = 0;




#ifndef CHECK_GL_ERRORS
#define CHECK_GL_ERRORS
void
CheckGlErrors( const char* caller )
{
	unsigned int gle = glGetError();

	if( gle != GL_NO_ERROR )
	{
		fprintf( stderr, "GL Error discovered from caller %s: ", caller );
		switch (gle)
		{
			case GL_INVALID_ENUM:
				fprintf( stderr, "Invalid enum.\n" );
				break;
			case GL_INVALID_VALUE:
				fprintf( stderr, "Invalid value.\n" );
				break;
			case GL_INVALID_OPERATION:
				fprintf( stderr, "Invalid Operation.\n" );
				break;
			case GL_STACK_OVERFLOW:
				fprintf( stderr, "Stack overflow.\n" );
				break;
			case GL_STACK_UNDERFLOW:
				fprintf(stderr, "Stack underflow.\n" );
				break;
			case GL_OUT_OF_MEMORY:
				fprintf( stderr, "Out of memory.\n" );
				break;
		}
		return;
	}
}
#endif



void
GLSLProgram::SaveProgramBinary( const char * fileName, GLenum * format )
{
	glProgramParameteri( this->Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE );
	GLint length;
	glGetProgramiv( this->Program, GL_PROGRAM_BINARY_LENGTH, &length );
	GLubyte *buffer = new GLubyte[length];
	glGetProgramBinary( this->Program, length, NULL, format, buffer );

	fprintf( stderr, "Program binary format = 0x%04x\n", *format );

	FILE * fpout = fopen( fileName, "wb" );
	if( fpout == NULL )
	{
		fprintf( stderr, "Cannot create output GLSL binary file '%s'\n", fileName );
		return;
	}
	fwrite( buffer, length, 1, fpout );
	fclose( fpout );
	delete [ ] buffer;
}


void
GLSLProgram::LoadProgramBinary( const char * fileName, GLenum format )
{
	FILE *fpin = fopen( fileName, "rb" );
	if( fpin == NULL )
	{
		fprintf( stderr, "Cannot open input GLSL binary file '%s'\n", fileName );
		return;
	}
	fseek( fpin, 0, SEEK_END );
	GLint length = (GLint)ftell( fpin );
	GLubyte *buffer = new GLubyte[ length ];
	rewind( fpin );
	fread( buffer, length, 1, fpin );
	fclose( fpin );

	glProgramBinary( this->Program, format, buffer, length );
	delete [ ] buffer;

	GLint   success;
	glGetProgramiv( this->Program, GL_LINK_STATUS, &success );

	if( !success )
	{
		fprintf( stderr, "Did not successfully load the GLSL binary file '%s'\n", fileName );
		return;
	}
}

void	GLSLProgram::EnableVertexAttribute(const string& name, const int vecLength /*=4*/) {
	const int loc = GetAttributeLocation(name);
	if (loc >= 0) {
		this->Use();
		glEnableVertexAttribArray( loc );
		glVertexAttribPointer( loc, vecLength, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
	}
}

void GLSLProgram::EnablePositionAttribute( const int vecLength /*= 4 */ ) {
	EnableVertexAttribute(POSITION_ATTRIBUTE_VAR, vecLength);
}

void GLSLProgram::EnableNormalAttribute( const int vecLength /*= 4 */ ) {
	EnableVertexAttribute(NORMAL_ATTRIBUTE_VAR, vecLength);
}

void GLSLProgram::EnableColorAttribute( const int vecLength /*= 4 */ ) {
	EnableVertexAttribute(COLOR_ATTRIBUTE_VAR, vecLength);
}

void GLSLProgram::EnableTexCoordAttribute( const int vecLength /*= 4 */ ) {
	EnableVertexAttribute(TEXCOORD_ATTRIBUTE_VAR, vecLength);
}


void GLSLProgram::LookAt( const vec3<>& eye, const vec3<>& at, const vec3<>& up ) {
	_mvMatrix = _mvMatrix * Angel::LookAt(eye, at, up);
	SendMVMatrix();
}

void GLSLProgram::Ortho( float left, float right, float bottom, float top, 
float zNear, float zFar )
{
	_projMatrix = _projMatrix * Angel::Ortho(left, right, bottom, top, zNear, zFar);
	SendProjMatrix();
}

void GLSLProgram::Perspective(float fovy, float ratio, float zNear, float zFar) {
	_projMatrix = _projMatrix * Angel::Perspective(fovy, ratio, zNear, zFar);
	SendProjMatrix();
}


void GLSLProgram::Rotate(float x, float y, float z) {
	_mvMatrix = _mvMatrix *  Angel::RotateX(x) * Angel::RotateY(y) * Angel::RotateZ(z);
	SendMVMatrix();
}

void GLSLProgram::Translate( float x, float y, float z ) {
	_mvMatrix = _mvMatrix * Angel::Translate(vec4<>(x,y,z,0.0));
	SendMVMatrix();
}

void GLSLProgram::Scale( float x, float y, float z ) {
	_mvMatrix = _mvMatrix * Angel::Scale(vec3<>(x,y,z));
	SendMVMatrix();
}

void GLSLProgram::ClearProjection() {
	_projMatrix = mat4();
}

void GLSLProgram::ClearModelView() {
	_mvMatrix = mat4();
}


void
GLSLProgram::SetGstap( bool b )
{
	IncludeGstap = b;
}


GLchar *Gstap = 
{
"#ifndef GSTAP_H\n\
#define GSTAP_H\n\
\n\
\n\
// gstap.h -- useful for glsl migration\n\
// from:\n\
//		Mike Bailey and Steve Cunningham\n\
//		\"Graphics Shaders: Theory and Practice\",\n\
//		Second Edition, AK Peters, 2011.\n\
\n\
\n\
\n\
// we are assuming that the compatibility #version line\n\
// is given in the source file, for example:\n\
// #version 400 compatibility\n\
\n\
\n\
// uniform variables:\n\
\n\
#define uModelViewMatrix		gl_ModelViewMatrix\n\
#define uProjectionMatrix		gl_ProjectionMatrix\n\
#define uModelViewProjectionMatrix	gl_ModelViewProjectionMatrix\n\
#define uNormalMatrix			gl_NormalMatrix\n\
#define uModelViewMatrixInverse		gl_ModelViewMatrixInverse\n\
\n\
// attribute variables:\n\
\n\
#define aColor				gl_Color\n\
#define aNormal				gl_Normal\n\
#define aVertex				gl_Vertex\n\
\n\
#define aTexCoord0			gl_MultiTexCoord0\n\
#define aTexCoord1			gl_MultiTexCoord1\n\
#define aTexCoord2			gl_MultiTexCoord2\n\
#define aTexCoord3			gl_MultiTexCoord3\n\
#define aTexCoord4			gl_MultiTexCoord4\n\
#define aTexCoord5			gl_MultiTexCoord5\n\
#define aTexCoord6			gl_MultiTexCoord6\n\
#define aTexCoord7			gl_MultiTexCoord7\n\
\n\
\n\
#endif		// #ifndef GSTAP_H\n\
\n\
\n"
};
