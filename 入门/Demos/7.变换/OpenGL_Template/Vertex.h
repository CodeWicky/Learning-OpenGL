#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in float show;
layout (location = 3) in vec2 aTexCoord;
out vec3 ourColor;
out float Img;
out vec2 TexCoord;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
    ourColor = aColor;
    Img = show;
    TexCoord = aTexCoord;
}
