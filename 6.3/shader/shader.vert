#version 330 core

in vec2 position;
out vec3 color;

void main()
{
    color = vec3(0.5, 0.0, 0.0);
    gl_Position = vec4(position, 0.0, 1.0);
}
