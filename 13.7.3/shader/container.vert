#version 330 core

in vec3 position;
in vec3 aNormal;
out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

void main()
{
    Normal = aNormal;
    FragPos = vec3(view * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    LightPos = vec3(view * vec4(lightPos, 1.0));
    gl_Position = projection * view * model * vec4(position, 1.0);
}
