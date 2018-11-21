#version 330 core
out vec4 FragColor;
in vec3 ourColor;
uniform vec3 lightColor;
void main()
{
    FragColor = vec4(ourColor * lightColor , 1.0);
}

