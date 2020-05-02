#version 330 core

in vec2 position;
in vec3 color;
in vec2 tex_coord;
out vec3 ourColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 0.0, 1.0);
    ourColor = color;
    texCoord = tex_coord;
}
