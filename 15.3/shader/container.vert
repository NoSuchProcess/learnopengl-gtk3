#version 330 core

in vec3 position;
in vec3 aNormal;
in vec2 aTexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    Normal = aNormal;
    FragPos = vec3(model * vec4(position, 1.0));
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
