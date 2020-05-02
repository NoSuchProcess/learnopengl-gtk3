#version 330 core

in vec3 fragment;
in vec3 normal;
in vec2 texcoord;
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texcoord));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragment);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texcoord));

    vec3 viewDir = normalize(viewPos - fragment);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
