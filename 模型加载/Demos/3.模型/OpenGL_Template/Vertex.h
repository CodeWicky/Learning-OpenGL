#version 330 core
///顶点坐标
layout (location = 0) in vec3 aPos;
///顶点法向量
layout (location = 1) in vec3 aNormal;
///顶点纹理坐标
layout (location = 2) in vec2 aTexCoord;
///模型矩阵
uniform mat4 model;
///观察矩阵
uniform mat4 view;
///裁剪矩阵
uniform mat4 projection;
out vec3 normal;
out vec3 fragPosition;
out vec2 TexCoord;
void main()
{
    ///顶点裁剪坐标系坐标
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ///顶点世界坐标
    fragPosition = vec3(model * vec4(aPos,1.0));
    ///经矩阵运算的法向量（防止模型变换后导致法向量不垂直）
    normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
}
