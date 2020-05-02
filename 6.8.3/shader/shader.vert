#version 330 core

in vec2 position;
in vec3 color;
out vec3 ourColor;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    ourColor = vec3(position.x + 0.5, position.y + 0.5, 0.0);
}
