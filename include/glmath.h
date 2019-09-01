#include <math.h>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

#define EPSILON 1.0e-6

typedef struct {
	GLfloat x, y;
} vec2;

typedef struct {
	GLfloat x, y, z;
} vec3;

typedef struct {
	GLfloat x, y, z, w;
} vec4;

typedef struct {
	GLfloat a11, a21;
	GLfloat a12, a22;
} mat2;

typedef struct {
	GLfloat a11, a21, a31;
	GLfloat a12, a22, a32;
	GLfloat a13, a23, a33;
} mat3;

typedef struct {
	GLfloat a11, a21, a31, a41;
	GLfloat a12, a22, a32, a42;
	GLfloat a13, a23, a33, a43;
	GLfloat a14, a24, a34, a44;
} mat4;


static inline GLfloat to_radians(const GLfloat degrees)
{
	return degrees * (PI / 180.0);
}

static inline GLfloat to_degrees(const GLfloat radians)
{
	return radians * (180.0 / PI);
}

static inline vec2 vec2_neg(const vec2 a)
{
	return (vec2) { .x = -a.x, .y = -a.y };
}

static inline vec3 vec3_neg(const vec3 a)
{
	return (vec3) { .x = -a.x, .y = -a.y, .z = -a.z };
}

static inline vec4 vec4_neg(const vec4 a)
{
	return (vec4) { .x = -a.x, .y = -a.y, .z = -a.z, .w = -a.w };
}

static inline vec2 vec2_add(const vec2 a, const vec2 b)
{
	return (vec2) { .x = a.x + b.x, .y = a.y + b.y };
}

static inline vec3 vec3_add(const vec3 a, const vec3 b)
{
	return (vec3) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
}

static inline vec4 vec4_add(const vec4 a, const vec4 b)
{
	return (vec4) { .x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z, .w = a.w + b.w };
}

static inline vec2 vec2_sub(const vec2 a, const vec2 b)
{
	return (vec2) { .x = a.x - b.x, .y = a.y - b.y };
}

static inline vec3 vec3_sub(const vec3 a, const vec3 b)
{
	return (vec3) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z };
}

static inline vec4 vec4_sub(const vec4 a, const vec4 b)
{
	return (vec4) { .x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z, .w = a.w - b.w };
}

static inline GLfloat vec2_dot(const vec2 a, const vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

static inline GLfloat vec3_dot(const vec3 a, const vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline GLfloat vec4_dot(const vec4 a, const vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

static inline vec3 vec3_mulf(const vec3 a, const GLfloat value)
{
	return (vec3) { a.x * value, a.y * value, a.z * value };
}

static inline vec3 vec3_mul(const vec3 a, const vec3 b)
{
	return (vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

static inline vec3 vec3_cross(const vec3 a, const vec3 b)
{
	return (vec3) {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
}

static inline GLfloat vec2_length(const vec2 a)
{
	return sqrt(vec2_dot(a, a));
}

static inline GLfloat vec3_length(const vec3 a)
{
	return sqrt(vec3_dot(a, a));
}

static inline GLfloat vec4_length(const vec4 a)
{
	return sqrt(vec4_dot(a, a));
}

static inline vec2 vec2_normalize(const vec2 a)
{
	const GLfloat length = vec2_length(a);

	if (length <= EPSILON) {
		return (vec2) { .x = 0, .y = 0 };
	}
	return (vec2) { .x = a.x / length, .y = a.y / length };
}

static inline vec3 vec3_normalize(const vec3 a)
{
	const GLfloat length = vec3_length(a);

	if (length <= EPSILON) {
		return (vec3) { .x = 0, .y = 0, .z = 0 };
	}
	return (vec3) { .x = a.x / length, .y = a.y / length, .z = a.z / length };
}

static inline vec4 vec4_normalize(const vec4 a)
{
	const GLfloat length = vec4_length(a);

	if (length <= EPSILON) {
		return (vec4) { .x = 0, .y = 0, .z = 0, .w = 0 };
	}
	return (vec4) { .x = a.x / length, .y = a.y / length, .z = a.z / length, .w = a.w / length };
}

static inline mat4 mat4_add(const mat4 A, const mat4 B)
{
	return (mat4) {
		.a11 = A.a11 + B.a11, .a12 = A.a12 + B.a12, .a13 = A.a13 + B.a13, .a14 = A.a14 + B.a14,
		.a21 = A.a21 + B.a21, .a22 = A.a22 + B.a22, .a23 = A.a23 + B.a23, .a24 = A.a24 + B.a24,
		.a31 = A.a31 + B.a31, .a32 = A.a32 + B.a32, .a33 = A.a33 + B.a33, .a34 = A.a34 + B.a34,
		.a41 = A.a41 + B.a41, .a42 = A.a42 + B.a42, .a43 = A.a43 + B.a43, .a44 = A.a44 + B.a44
	};
}

static inline mat4 mat4_sub(const mat4 A, const mat4 B)
{
	return (mat4) {
		.a11 = A.a11 - B.a11, .a12 = A.a12 - B.a12, .a13 = A.a13 - B.a13, .a14 = A.a14 - B.a14,
		.a21 = A.a21 - B.a21, .a22 = A.a22 - B.a22, .a23 = A.a23 - B.a23, .a24 = A.a24 - B.a24,
		.a31 = A.a31 - B.a31, .a32 = A.a32 - B.a32, .a33 = A.a33 - B.a33, .a34 = A.a34 - B.a34,
		.a41 = A.a41 - B.a41, .a42 = A.a42 - B.a42, .a43 = A.a43 - B.a43, .a44 = A.a44 - B.a44
	};
}

#define ROW(M, row) ((vec4) { M.a##row##1, M.a##row##2, M.a##row##3, M.a##row##4 })
#define COL(M, col) ((vec4) { M.a1##col, M.a2##col, M.a3##col, M.a4##col })

static inline mat4 mat4_mul(const mat4 A, const mat4 B)
{
	return (mat4) {
		.a11 = vec4_dot(ROW(A, 1), COL(B, 1)),
		.a12 = vec4_dot(ROW(A, 1), COL(B, 2)),
		.a13 = vec4_dot(ROW(A, 1), COL(B, 3)),
		.a14 = vec4_dot(ROW(A, 1), COL(B, 4)),

		.a21 = vec4_dot(ROW(A, 2), COL(B, 1)),
		.a22 = vec4_dot(ROW(A, 2), COL(B, 2)),
		.a23 = vec4_dot(ROW(A, 2), COL(B, 3)),
		.a24 = vec4_dot(ROW(A, 2), COL(B, 4)),

		.a31 = vec4_dot(ROW(A, 3), COL(B, 1)),
		.a32 = vec4_dot(ROW(A, 3), COL(B, 2)),
		.a33 = vec4_dot(ROW(A, 3), COL(B, 3)),
		.a34 = vec4_dot(ROW(A, 3), COL(B, 4)),

		.a41 = vec4_dot(ROW(A, 4), COL(B, 1)),
		.a42 = vec4_dot(ROW(A, 4), COL(B, 2)),
		.a43 = vec4_dot(ROW(A, 4), COL(B, 3)),
		.a44 = vec4_dot(ROW(A, 4), COL(B, 4))
	};
}

static inline mat4 mat4_identity()
{
	return (mat4) {
		.a11 = 1.0, .a12 = 0, .a13 = 0, .a14 = 0,
		.a21 = 0, .a22 = 1.0, .a23 = 0, .a24 = 0,
		.a31 = 0, .a32 = 0, .a33 = 1.0, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_scaling(const vec3 scale)
{
	return (mat4) {
		.a11 = scale.x, .a12 = 0, .a13 = 0, .a14 = 0,
		.a21 = 0, .a22 = scale.y, .a23 = 0, .a24 = 0,
		.a31 = 0, .a32 = 0, .a33 = scale.z, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_translation(const vec3 translate)
{
	return (mat4) {
		.a11 = 1.0, .a12 = 0, .a13 = 0, .a14 = translate.x,
		.a21 = 0, .a22 = 1.0, .a23 = 0, .a24 = translate.y,
		.a31 = 0, .a32 = 0, .a33 = 1.0, .a34 = translate.z,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_transformation(const vec3 scale, const vec3 translate)
{
	return (mat4) {
		.a11 = scale.x, .a12 = 0, .a13 = 0, .a14 = translate.x,
		.a21 = 0, .a22 = scale.y, .a23 = 0, .a24 = translate.y,
		.a31 = 0, .a32 = 0, .a33 = scale.z, .a34 = translate.z,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_rotation_x(const GLfloat angle)
{
	const GLfloat sin_a = sin(angle);
	const GLfloat cos_a = cos(angle);

	return (mat4) {
		.a11 = 1.0, .a12 = 0, .a13 = 0, .a14 = 0,
		.a21 = 0, .a22 = cos_a, .a23 = -sin_a, .a24 = 0,
		.a31 = 0, .a32 = sin_a, .a33 = cos_a, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_rotation_y(const GLfloat angle)
{
	const GLfloat sin_a = sin(angle);
	const GLfloat cos_a = cos(angle);

	return (mat4) {
		.a11 = cos_a, .a12 = 0, .a13 = sin_a, .a14 = 0,
		.a21 = 0, .a22 = 1.0, .a23 = 0, .a24 = 0,
		.a31 = -sin_a, .a32 = 0, .a33 = cos_a, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_rotation_z(const GLfloat angle)
{
	const GLfloat sin_a = sin(angle);
	const GLfloat cos_a = cos(angle);

	return (mat4) {
		.a11 = cos_a, .a12 = -sin_a, .a13 = 0, .a14 = 0,
		.a21 = sin_a, .a22 = cos_a, .a23 = 0, .a24 = 0,
		.a31 = 0, .a32 = 0, .a33 = 1.0, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_rotation(const GLfloat angle, const vec3 axis)
{
	const GLfloat sin_a = sin(angle);
	const GLfloat cos_a = cos(angle);
	const vec3 unit = vec3_normalize(axis);

	return (mat4) {
		.a11 = unit.x * unit.x * (1 - cos_a) + cos_a,
		.a12 = unit.x * unit.y * (1 - cos_a) - unit.z * sin_a,
		.a13 = unit.x * unit.z * (1 - cos_a) + unit.y * sin_a,
		.a14 = 0,

		.a21 = unit.y * unit.x * (1 - cos_a) + unit.z * sin_a,
		.a22 = unit.y * unit.y * (1 - cos_a) + cos_a,
		.a23 = unit.y * unit.z * (1 - cos_a) - unit.x * sin_a,
		.a24 = 0,

		.a31 = unit.z * unit.x * (1 - cos_a) - unit.y * sin_a,
		.a32 = unit.z * unit.y * (1 - cos_a) + unit.x * sin_a,
		.a33 = unit.z * unit.z * (1 - cos_a) + cos_a,
		.a34 = 0,

		.a41 = 0,
		.a42 = 0,
		.a43 = 0,
		.a44 = 1.0
	};
}

static inline mat4 mat4_ortho(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near, GLfloat far)
{
	return (mat4) {
		.a11 = 2.0 / (right - left), .a12 = 0, .a13 = 0, .a14 = -(right + left) / (right - left),
		.a21 = 0, .a22 = 2.0 / (top - bottom), .a23 = 0, .a24 = -(top + bottom) / (top - bottom),
		.a31 = 0, .a32 = 0, .a33 = -2.0 / (far - near), .a34 = -(far + near) / (far - near),
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
}

static inline mat4 mat4_perspective(GLfloat fov, GLfloat aspect, GLfloat near, GLfloat far)
{
	const GLfloat tan_fov_2 = tan(fov / 2.0);

	return (mat4) {
		.a11 = 1.0 / (aspect * tan_fov_2), .a12 = 0, .a13 = 0, .a14 = 0,
		.a21 = 0, .a22 = 1.0 / tan_fov_2, .a23 = 0, .a24 = 0,
		.a31 = 0, .a32 = 0, .a33 = -(far + near) / (far - near), .a34 = -2.0 * far * near / (far - near),
		.a41 = 0, .a42 = 0, .a43 = -1.0, .a44 = 0
	};
}

static inline mat4 mat4_look_at(vec3 position, vec3 target, vec3 up)
{
	const vec3 direction = vec3_normalize(vec3_sub(position, target));
	const vec3 right = vec3_normalize(vec3_cross(up, direction));
	const vec3 cam_up = vec3_cross(direction, right);
	const mat4 look = {
		.a11 = right.x, .a12 = right.y, .a13 = right.z, .a14 = 0,
		.a21 = cam_up.x, .a22 = cam_up.y, .a23 = cam_up.z, .a24 = 0,
		.a31 = direction.x, .a32 = direction.y, .a33 = direction.z, .a34 = 0,
		.a41 = 0, .a42 = 0, .a43 = 0, .a44 = 1.0
	};
	return mat4_mul(look, mat4_translation(vec3_neg(position)));
}

static inline vec3 vec3_mulm(const mat4 m, const vec3 v)
{
	return (vec3) {
		.x = m.a11 * v.x + m.a12 * v.y + m.a13 * v.z,
		.y = m.a21 * v.x + m.a22 * v.y + m.a23 * v.z,
		.z = m.a31 * v.x + m.a32 * v.y + m.a33 * v.z
	};
}

static inline void mat2_print(const mat2 m)
{
	g_print("[%lf %lf]\n", m.a11, m.a12);
	g_print("[%lf %lf]\n", m.a21, m.a22);
}

static inline void mat3_print(const mat3 m)
{
	g_print("[%lf %lf %lf]\n", m.a11, m.a12, m.a13);
	g_print("[%lf %lf %lf]\n", m.a21, m.a22, m.a23);
	g_print("[%lf %lf %lf]\n", m.a31, m.a32, m.a33);
}

static inline void mat4_print(const mat4 m)
{
	g_print("[%lf %lf %lf %lf]\n", m.a11, m.a12, m.a13, m.a14);
	g_print("[%lf %lf %lf %lf]\n", m.a21, m.a22, m.a23, m.a24);
	g_print("[%lf %lf %lf %lf]\n", m.a31, m.a32, m.a33, m.a34);
	g_print("[%lf %lf %lf %lf]\n", m.a41, m.a42, m.a43, m.a44);
}
