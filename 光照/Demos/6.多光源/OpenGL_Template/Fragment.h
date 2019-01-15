#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3  direction;
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 6

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
uniform Light pointLight;
uniform Light directionLight;
uniform Light spotLights[NR_POINT_LIGHTS];



///计算定向光的光照颜色（光源、物体材质、观察点）
vec3 CalculateDirectionLight(Light aLight,Material aMaterial,vec3 viewPos);
///计算点光源的光照颜色（光源、物体材质、观察点）
vec3 CalculatePointLight(Light aLight,Material aMaterial,vec3 viewPos);
///计算聚光光源的光照颜色（光源、物体材质、观察点）
vec3 CalculateSpotLight(Light aLight,Material aMaterial,vec3 viewPos);


void main()
{
//    ///环境光照
//    vec3 ambientColor = light.ambient * vec3(texture(material.diffuse, TexCoord));
//
//    ///计算到顶点光线方向
//    vec3 norm = normalize(normal);
//    vec3 lightDir = normalize(light.position - fragPosition);
//
//    ///计算衰减系数r
//    float distance = length(light.position - fragPosition);
//    float attenuationFactor = 1.0 / (light.constant + light.linear * distance +
//                                     light.quadratic * (distance * distance));
//
//    float theta = dot(lightDir, normalize(-light.direction));
//    float smoothFactor = clamp((theta - light.outerCutOff)/(light.cutOff - light.outerCutOff),0.0,1.0);
//
//    ///漫反射光照
//    float factor = max(dot(norm,lightDir),0.0);
//    vec3 diffuseColor = light.diffuse * factor * vec3(texture(material.diffuse, TexCoord));
//
//    ///镜面反射光照
//    vec3 reflectDir = reflect(-lightDir, norm);
//    vec3 viewDir = normalize(viewPosition - fragPosition);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0),material.shininess);
//    vec3 specularColor = light.specular * spec * vec3(texture(material.specular, TexCoord));
    
    vec3 DirL = CalculatePointLight(pointLight,material,viewPosition);
    DirL += CalculateDirectionLight(directionLight,material,viewPosition);
    ///颜色合成
    FragColor = vec4(DirL , 1.0);
}

///定向光源
vec3 CalculateDirectionLight(Light aLight,Material aMaterial,vec3 viewPos) {
    ///环境光照
    vec3 ambientColor = aLight.ambient * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///计算到顶点光线方向
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-aLight.direction);
    
    ///计算衰减系数r
    float distance = length(aLight.position - fragPosition);
    float attenuationFactor = 1.0 / (aLight.constant + aLight.linear * distance +
                                     aLight.quadratic * (distance * distance));
    
    ///漫反射光照
    float factor = max(dot(norm,lightDir),0.0);
    vec3 diffuseColor = aLight.diffuse * factor * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///镜面反射光照
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),aMaterial.shininess);
    vec3 specularColor = aLight.specular * spec * vec3(texture(aMaterial.specular, TexCoord));
    return (ambientColor + diffuseColor + specularColor) * attenuationFactor;
}

///点光源
vec3 CalculatePointLight(Light aLight,Material aMaterial,vec3 viewPos) {
    vec3 ambientColor = aLight.ambient * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///计算到顶点光线方向
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(aLight.position - fragPosition);
    
    ///计算衰减系数r
    float distance = length(aLight.position - fragPosition);
    float attenuationFactor = 1.0 / (aLight.constant + aLight.linear * distance +
                                     aLight.quadratic * (distance * distance));
    
    ///漫反射光照
    float factor = max(dot(norm,lightDir),0.0);
    vec3 diffuseColor = aLight.diffuse * factor * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///镜面反射光照
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),aMaterial.shininess);
    vec3 specularColor = aLight.specular * spec * vec3(texture(aMaterial.specular, TexCoord));
    return (ambientColor + diffuseColor + specularColor) * attenuationFactor;
}


///聚光光源
vec3 CalculateSpotLight(Light aLight,Material aMaterial,vec3 viewPos) {
    ///环境光照
    vec3 ambientColor = aLight.ambient * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///计算到顶点光线方向
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(aLight.position - fragPosition);
    
    ///计算衰减系数r
    float distance = length(aLight.position - fragPosition);
    float attenuationFactor = 1.0 / (aLight.constant + aLight.linear * distance +
                                     aLight.quadratic * (distance * distance));
    
    ///计算柔化系数
    float theta = dot(lightDir, normalize(-aLight.direction));
    float smoothFactor = clamp((theta - aLight.outerCutOff)/(aLight.cutOff - aLight.outerCutOff),0.0,1.0);
    
    ///漫反射光照
    float factor = max(dot(norm,lightDir),0.0);
    vec3 diffuseColor = aLight.diffuse * factor * vec3(texture(aMaterial.diffuse, TexCoord));
    
    ///镜面反射光照
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - fragPosition);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),aMaterial.shininess);
    vec3 specularColor = aLight.specular * spec * vec3(texture(aMaterial.specular, TexCoord));
    return (ambientColor + (diffuseColor + specularColor) * smoothFactor) * attenuationFactor;
}


