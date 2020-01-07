#include <math.h>
#include <stdio.h>

#define PI   3.141592653589793238462643383279502884197169399375105820975
#define PI_2 1.570796326794896619231321691639751442098584699687552910487

#define N 32

typedef struct {
	double x, y;
} vec2;

typedef struct {
	double x, y, z;
} vec3;

static vec3 vec3_rotate_x(const vec3 v, const double angle)
{
	return (vec3) {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};
}

static vec3 vec3_rotate_y(const vec3 v, const double angle)
{
	return (vec3) {
		.x = v.x * cos(angle) + v.z * sin(angle),
		.y = v.y,
		.z = -v.x * sin(angle) + v.z * cos(angle)
	};
}

static vec3 vec3_rotate_z(const vec3 v, const double angle)
{
	return (vec3) {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};
}

static vec3 vec3_divf(const vec3 v, const double a)
{
	return (vec3) {
		.x = v.x / a,
		.y = v.y / a,
		.z = v.z / a
	};
}

double vec3_dot(const vec3 a, const vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 vec3_cross(const vec3 a, const vec3 b)
{
	return (vec3) {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
}

double vec3_length(const vec3 v)
{
        return sqrt(vec3_dot(v, v));
}

static vec3 vec3_normalize(const vec3 v)
{
	return vec3_divf(v, vec3_length(v));
}


static void vertex_print(const vec3 v, const vec3 n, const vec2 t)
{
	printf("\t{{%+lf, %+lf, %+lf}, {%+lf, %+lf, %+lf}, {%lf, %lf}}", v.x, v.y, v.z, n.x, n.y, n.z, t.x, t.y);
}

int main(int argc, char *argv[])
{
	vec3 point1 = (vec3) {1.0, -1.0, 0.0}; 
	vec3 point2 = (vec3) {1.0, +1.0, 0.0}; 

	/* vertices */
	puts("static vertex_t vertices[] = {");
	for (size_t i = 0; i <= N; ++i) {
		vertex_print(point1, vec3_normalize((vec3) {point1.x, 0.0, point1.z}), (vec2) {(double) i / (double) N, 1.0});
		puts(",");
		vertex_print(point2, vec3_normalize((vec3) {point2.x, 0.0, point2.z}), (vec2) {(double) i / (double) N, 0.0});
		puts(i < N ? "," : "");

		point1 = vec3_rotate_y(point1, (2.0 * PI / N));
		point2 = vec3_rotate_y(point2, (2.0 * PI / N));
	}
	puts("};\n");

/*
	0-2-4... 2N
	|/|/|/
	1-3-5... 2N+1
*/
	/* indices */
	puts("static GLuint indices[] = {");
	for (size_t i = 0; i < N; ++i) {
		size_t ndx[3];

		ndx[0] = i * 2 + 1;
		ndx[1] = i * 2 + 2;
		ndx[2] = i * 2 + 0;
		printf("\t%u, %u, %u,\n", ndx[0], ndx[1], ndx[2]);

		ndx[0] = i * 2 + 1;
		ndx[1] = i * 2 + 3;
		ndx[2] = i * 2 + 2;
		printf("\t%u, %u, %u", ndx[0], ndx[1], ndx[2]);

		puts(i < N ? "," : "");
	}
	puts("};");

	return 0;
}
