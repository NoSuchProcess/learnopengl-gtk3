#ifndef __SHADER_MAKE_H__
#define __SHADER_MAKE_H__

#include <epoxy/gl.h>

typedef enum {
	SHADER_SET_CONTAINER,
	SHADER_SET_LIGHT,
} shader_set;

GLuint shader_make(void);

#endif
