//////////////////////////////////////////////////////////////////////////////
//
//  --- vec.h ---
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Angel.h"

namespace Angel {

//////////////////////////////////////////////////////////////////////////////
//
//  vec2.h - 2D vector
//
template <class T = GLfloat>
struct vec2 {

    T  x;
    T  y;

    //
    //  --- Constructors and Destructors ---
    //

    vec2( T s = T(0.0) ) :
	x(s), y(s) {}

    vec2( T x, T y ) :
	x(x), y(y) {}

    vec2( const vec2& v )
	{ x = v.x;  y = v.y;  }

    //
    //  --- Indexing Operator ---
    //

    T& operator [] ( int i ) { return *(&x + i); }
    const T operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec2 operator - () const // unary minus operator
	{ return vec2( -x, -y ); }

    vec2 operator + ( const vec2& v ) const
	{ return vec2( x + v.x, y + v.y ); }

    vec2 operator - ( const vec2& v ) const
	{ return vec2( x - v.x, y - v.y ); }

    vec2 operator * ( const T s ) const
	{ return vec2( s*x, s*y ); }

    vec2 operator * ( const vec2& v ) const
	{ return vec2( x*v.x, y*v.y ); }

    friend vec2 operator * ( const T s, const vec2& v )
	{ return v * s; }

    vec2 operator / ( const T s ) const {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	    return vec2();
	}
#endif // DEBUG

	T r = T(1.0) / s;
	return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec2& operator += ( const vec2& v )
	{ x += v.x;  y += v.y;   return *this; }

    vec2& operator -= ( const vec2& v )
	{ x -= v.x;  y -= v.y;  return *this; }

    vec2& operator *= ( const T s )
	{ x *= s;  y *= s;   return *this; }

    vec2& operator *= ( const vec2& v )
	{ x *= v.x;  y *= v.y; return *this; }

    vec2& operator /= ( const T s ) {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	}
#endif // DEBUG

	T r = T(1.0) / s;
	*this *= r;

	return *this;
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec2& v ) {
	return os << "( " << v.x << ", " << v.y <<  " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec2& v )
	{ return is >> v.x >> v.y ; }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat* () const
	{ return static_cast<const GLfloat*>( &x ); }

    operator GLfloat* ()
	{ return static_cast<GLfloat*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec2 Methods
//

template <class T> inline
T dot( const vec2<T>& u, const vec2<T>& v ) {
    return u.x * v.x + u.y * v.y;
}

template <class T> inline
T length( const vec2<T>& v ) {
    return std::sqrt( dot(v,v) );
}

template <class T> inline
vec2<T> normalize( const vec2<T>& v ) {
    return v / length(v);
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////
template <class T = GLfloat>
struct vec3 {

    T  x;
    T  y;
    T  z;

    //
    //  --- Constructors and Destructors ---
    //

    vec3( T s = T(0.0) ) :
	x(s), y(s), z(s) {}

    vec3( T x, T y, T z ) :
	x(x), y(y), z(z) {}

    vec3( const vec3& v ) { x = v.x;  y = v.y;  z = v.z; }

    vec3( const vec2<T>& v, const float f ) { x = v.x;  y = v.y;  z = f; }

    //
    //  --- Indexing Operator ---
    //

    GLfloat& operator [] ( int i ) { return *(&x + i); }
    const GLfloat operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec3 operator - () const  // unary minus operator
	{ return vec3( -x, -y, -z ); }

    vec3 operator + ( const vec3& v ) const
	{ return vec3( x + v.x, y + v.y, z + v.z ); }

    vec3 operator - ( const vec3& v ) const
	{ return vec3( x - v.x, y - v.y, z - v.z ); }

    vec3 operator * ( const GLfloat s ) const
	{ return vec3( s*x, s*y, s*z ); }

    vec3 operator * ( const vec3& v ) const
	{ return vec3( x*v.x, y*v.y, z*v.z ); }

    friend vec3 operator * ( const GLfloat s, const vec3& v )
	{ return v * s; }

    vec3 operator / ( const GLfloat s ) const {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	    return vec3();
	}
#endif // DEBUG

	GLfloat r = GLfloat(1.0) / s;
	return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec3& operator += ( const vec3& v )
	{ x += v.x;  y += v.y;  z += v.z;  return *this; }

    vec3& operator -= ( const vec3& v )
	{ x -= v.x;  y -= v.y;  z -= v.z;  return *this; }

    vec3& operator *= ( const GLfloat s )
	{ x *= s;  y *= s;  z *= s;  return *this; }

    vec3& operator *= ( const vec3& v )
	{ x *= v.x;  y *= v.y;  z *= v.z;  return *this; }

    vec3& operator /= ( const GLfloat s ) {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	}
#endif // DEBUG

	GLfloat r = GLfloat(1.0) / s;
	*this *= r;

	return *this;
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec3& v ) {
	return os << "( " << v.x << ", " << v.y << ", " << v.z <<  " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec3& v )
	{ return is >> v.x >> v.y >> v.z ; }

    //
    //  --- Conversion Operators ---
    //

    operator const GLfloat* () const
	{ return static_cast<const GLfloat*>( &x ); }

    operator GLfloat* ()
	{ return static_cast<GLfloat*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//

template <class T> inline
T dot( const vec3<T>& u, const vec3<T>& v ) {
    return u.x*v.x + u.y*v.y + u.z*v.z ;
}

template <class T> inline
T length( const vec3<T>& v ) {
    return std::sqrt( dot(v,v) );
}

template <class T> inline
vec3<T> normalize( const vec3<T>& v ) {
    return v / length(v);
}

template <class T> inline
vec3<T> cross(const vec3<T>& a, const vec3<T>& b )
{
    return vec3<T>( a.y * b.z - a.z * b.y,
		 a.z * b.x - a.x * b.z,
		 a.x * b.y - a.y * b.x );
}


//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////
template <class T = GLfloat>
struct vec4 {

    T  x;
    T  y;
    T  z;
    T  w;

    //
    //  --- Constructors and Destructors ---
    //

    vec4( T s = T(0.0) ) :
	x(s), y(s), z(s), w(s) {}

    vec4( T x, T y, T z, T w ) :
	x(x), y(y), z(z), w(w) {}

    vec4( const vec4& v ) { x = v.x;  y = v.y;  z = v.z;  w = v.w; }

    vec4( const vec3<>& v, const float w = 1.0 ) : w(w)
	{ x = v.x;  y = v.y;  z = v.z; }

    vec4( const vec2<>& v, const float z, const float w ) : z(z), w(w)
	{ x = v.x;  y = v.y; }

    //
    //  --- Indexing Operator ---
    //

    T& operator [] ( int i ) { return *(&x + i); }
    const T operator [] ( int i ) const { return *(&x + i); }

    //
    //  --- (non-modifying) Arithematic Operators ---
    //

    vec4 operator - () const  // unary minus operator
	{ return vec4( -x, -y, -z, -w ); }

    vec4 operator + ( const vec4& v ) const
	{ return vec4( x + v.x, y + v.y, z + v.z, w + v.w ); }

    vec4 operator - ( const vec4& v ) const
	{ return vec4( x - v.x, y - v.y, z - v.z, w - v.w ); }

    vec4 operator * ( const T s ) const
	{ return vec4( s*x, s*y, s*z, s*w ); }

    vec4 operator * ( const vec4& v ) const
	{ return vec4( x*v.x, y*v.y, z*v.z, w*v.z ); }

    friend vec4 operator * ( const T s, const vec4& v )
	{ return v * s; }

    vec4 operator / ( const T s ) const {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	    return vec4();
	}
#endif // DEBUG

	T r = T(1.0) / s;
	return *this * r;
    }

    //
    //  --- (modifying) Arithematic Operators ---
    //

    vec4& operator += ( const vec4& v )
	{ x += v.x;  y += v.y;  z += v.z;  w += v.w;  return *this; }

    vec4& operator -= ( const vec4& v )
	{ x -= v.x;  y -= v.y;  z -= v.z;  w -= v.w;  return *this; }

    vec4& operator *= ( const T s )
	{ x *= s;  y *= s;  z *= s;  w *= s;  return *this; }

    vec4& operator *= ( const vec4& v )
	{ x *= v.x, y *= v.y, z *= v.z, w *= v.w;  return *this; }

    vec4& operator /= ( const T s ) {
#ifdef DEBUG
	if ( std::fabs(s) < DivideByZeroTolerance ) {
	    std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
		      << "Division by zero" << std::endl;
	}
#endif // DEBUG

	T r = T(1.0) / s;
	*this *= r;

	return *this;
    }
	
    //
    //  --- Insertion and Extraction Operators ---
    //

    friend std::ostream& operator << ( std::ostream& os, const vec4& v ) {
	return os << "( " << v.x << ", " << v.y
		  << ", " << v.z << ", " << v.w << " )";
    }

    friend std::istream& operator >> ( std::istream& is, vec4& v )
	{ return is >> v.x >> v.y >> v.z >> v.w; }

    //
    //  --- Conversion Operators ---
    //

    operator const T* () const
	{ return static_cast<const T*>( &x ); }

    operator T* ()
	{ return static_cast<T*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec4 Methods
//

template <class T> inline
GLfloat dot( const vec4<T>& u, const vec4<T>& v ) {
    return u.x*v.x + u.y*v.y + u.z*v.z + u.w+v.w;
}

template <class T> inline
T length( const vec4<T>& v ) {
    return std::sqrt( dot(v,v) );
}

template <class T> inline
vec4<T> normalize( const vec4<T>& v ) {
    return v / length(v);
}

template <class T> inline
vec3<T> cross(const vec4<T>& a, const vec4<T>& b )
{
    return vec3<T>( a.y * b.z - a.z * b.y,
		 a.z * b.x - a.x * b.z,
		 a.x * b.y - a.y * b.x );
}

//----------------------------------------------------------------------------

}  // namespace Angel


