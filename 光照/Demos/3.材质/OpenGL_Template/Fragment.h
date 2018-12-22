#version 330 core
///输出颜色
out vec4 FragColor;
///顶点颜色
in vec3 ourColor;
///法向量
in vec3 normal;
///顶点世界坐标
in vec3 fragPosition;
///环境强度
uniform float ambientStrength;
///镜面强度
uniform float specularStrength;
///光照颜色
uniform vec3 lightColor;
///光照点
uniform vec3 lightPosition;
///观察点
uniform vec3 viewPosition;
void main()
{
    ///环境光照
    vec3 ambientColor = lightColor * (max(ambientStrength,0.0));
    
    ///漫反射光照
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float factor = max(dot(norm,lightDir),0.0);
    vec3 diffuseColor = lightColor * factor;
    
    ///镜面反射光照
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPosition - fragPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32);
    vec3 specularColor = specularStrength * spec * lightColor;
    
    ///颜色合成
    FragColor = vec4(ourColor * (ambientColor + diffuseColor + specularColor) , 1.0);
}


