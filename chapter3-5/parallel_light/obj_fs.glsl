#version 330 core
out vec4 FragColor;

/* 定义材质属性 */
// 移除了环境光材质颜色向量，是因为环境光颜色在几乎所有情况下都等于漫反射颜色
struct Material {
    sampler2D diffuse;  // 漫反射贴图
    sampler2D specular; // 镜面贴图
    float shininess;    // 反光度，镜面高光的散射/半径
};

/* 定义光源属性 */
struct Light {
    vec3 direction;         // 光的方向向量，从光源到物体

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;                                                              // 摄像机的位置向量
uniform Material material;
uniform Light light;

void main() {
    /* 环境光照 */
    vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;       // 环境光照分量

    /* 漫反射光照 */
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);                                   // 光的方向向量，从物体表面的某一点指向光源
    float diff = max(dot(norm, lightDir), 0.0);                                    // 漫反射光照的颜色强度
    vec3 diffuse = diff * texture(material.diffuse, TexCoords).rgb * light.diffuse;// 漫反射光照分量

    /* 镜面光照 */
    vec3 viewDir = normalize(viewPos - FragPos);                                   // 观察向量
    vec3 reflectDir = reflect(-lightDir, norm);                                    // 反射向量，reflect 函数要求第一个向量是从光源指向片段位置的向量，但是 lightDir 当前正好相反，所以需要取反
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;// 镜面光照分量

    /* 计算最终颜色 */
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}