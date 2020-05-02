#version 330 core

in vec3 vec_position;
in vec3 vec_normal;
in vec2 tex_coord;

out vec3 fragment;
out vec3 normal;
out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fragment = vec3(model * vec4(vec_position, 1.0));
    normal = vec3(model * vec4(vec_normal, 0.0));
    texcoord = tex_coord;
    gl_Position = projection * view * model * vec4(vec_position, 1.0);
}
