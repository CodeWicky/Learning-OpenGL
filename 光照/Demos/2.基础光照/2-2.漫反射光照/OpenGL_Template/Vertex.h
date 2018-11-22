#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 ourColor;
out vec3 normal;
out vec3 fragPosition;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    fragPosition = vec3(model * vec4(aPos,1.0));
    normal = aNormal;
}
