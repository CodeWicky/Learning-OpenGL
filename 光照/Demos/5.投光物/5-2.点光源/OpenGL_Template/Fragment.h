#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

///输出颜色
out vec4 FragColor;
///纹理坐标
in vec2 TexCoord;
///法向量
in vec3 normal;
///顶点世界坐标
in vec3 fragPosition;
///观察点
uniform vec3 viewPosition;
///材质
uniform Material material;
///光源
uniform Light light;
void main()
{
    ///环境光照
    vec3 ambientColor = light.ambient * vec3(texture(material.diffuse, TexCoord));
    
    ///漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPosition);
    float factor = max(dot(norm,lightDir),0.0);
    vec3 diffuseColor = light.diffuse * factor * vec3(texture(material.diffuse, TexCoord));
    
    ///镜面反射光照
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPosition - fragPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
    vec3 specularColor = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    ///颜色合成
    FragColor = vec4((ambientColor + diffuseColor + specularColor) , 1.0);
}


