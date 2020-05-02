#include <math.h>
#include <stdio.h>

#define PI   3.141592653589793238462643383279502884197169399375105820975
#define PI_2 1.570796326794896619231321691639751442098584699687552910487

#define N 32
#define M 64

#define R 0.6			// radius
#define W 0.4			// width

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

static void vertex_print(const vec3 v, const vec3 n, const vec2 t)
{
	printf("\t{{%+lf, %+lf, %+lf}, {%+lf, %+lf, %+lf}, {%lf, %lf}}", v.x, v.y, v.z, n.x, n.y, n.z, t.x, t.y);
}

/*
                     2
                  1     3
 o ------------  0,N     4
                  7     5
		     6
*/
int main(int argc, char *argv[])
{
	vec3 outline[N + 1];
	vec3 norm[N + 1];
	vec2 texture[N + 1];

	for (size_t i = 0; i <= N; ++i) {
		outline[i] = (vec3) {
			.x = R - W * cos(i * (2 * PI / N)),
			.y = W * sin(i * (2 * PI / N)),
			.z = 0.0
		};
		norm[i] = (vec3) {
			.x = -cos(i * (2 * PI / N)),
			.y = sin(i * (2 * PI / N)),
			.z = 0.0
		};
		texture[i] = (vec2) {
			.x = 0.0,
			.y = (double) i / (double) N
		};
	}

	/* vertices */
	puts("static vertex_t vertices[] = {");
	for (size_t i = 0; i <= M; ++i) {
		for (size_t j = 0; j <= N; ++j) {
			vertex_print(outline[j], norm[j], texture[j]);
			puts(!(i == M && j == N) ? "," : "");
			outline[j] = vec3_rotate_y(outline[j], 2 * PI / M);
			norm[j] = vec3_rotate_y(norm[j], 2 * PI / M);
			texture[j].x = (double) (i + 1) / (double) M;
		}
		if (i + 1 < M) {
			puts("");
		}
	}
	puts("};\n");

/*
    2 - N+3
    | \ |
    1 - N+2
    | \ |
    0 - N+1
    N - N+1
    | \ |
    N-1-N+N-1
*/
	/* indices */
	puts("static GLuint indices[] = {");
	for (size_t i = 0; i < M; ++i) {
		for (size_t j = 0; j < N; ++j) {
			size_t ndx[3];

			ndx[0] = ((j + 0) + i * (N + 1));
			ndx[1] = ((j + N + 1) + i * (N + 1));
			ndx[2] = ((j + 1) + i * (N + 1));
			printf("\t%u, %u, %u,\n", ndx[0], ndx[1], ndx[2]);

			ndx[0] = ((j + 1) + i * (N + 1));
			ndx[1] = ((j + N + 1) + i * (N + 1));
			ndx[2] = ((j + N + 2) + i * (N + 1));
			printf("\t%u, %u, %u", ndx[0], ndx[1], ndx[2]);

			if (!(i + 1 == M && j + 1 == N)) {
				puts(",");
			}
		}
		puts("");
	}
	puts("};");

	return 0;
}
