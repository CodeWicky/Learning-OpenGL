#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 normal;
in vec3 fragPosition;
uniform vec3 ambientColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
void main()
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float factor = max(dot(norm,lightDir),0.0);
    FragColor = vec4(ourColor * (ambientColor + lightColor * factor) , 1.0);
}

