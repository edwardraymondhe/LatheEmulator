#version 330 core
out vec4 FragColor;

// in vec3 ourColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec2 TexCoord;

in vec3 Normal;  
in vec3 FragPos;

uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

uniform int texIdx;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform sampler2D ourTexture;

void main()
{
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 skyboxColor = vec4(texture(skybox, R).rgb, 1.0);


    // ambient
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;  
    
    // newly added, replacing "in objectColor"
    vec4 objectColor = texture(ourTexture, TexCoord);

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0) * objectColor;

    if(texIdx != 0)
    {
        FragColor = FragColor * skyboxColor;
    }
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	// FragColor = texture(ourTexture, TexCoord);

}