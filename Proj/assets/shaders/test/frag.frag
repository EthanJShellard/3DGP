#version 410
uniform sampler2D u_Texture;
uniform mat4 u_View;
uniform vec3 u_camPos;
uniform vec3 u_lightPos;
uniform float u_dissolve;

varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_FragPos;

void main()
{
    //Light setup
    vec3 lightColor = vec3(1, 1, 1);
    vec3 light;

    //Normal setup
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_lightPos - v_FragPos);
    
    //Diffuse lighting setup
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //Ambient lighting setup
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    //Specular lighting
    vec3 viewDir = normalize(u_camPos - v_FragPos);
    //Phong reflect
    vec3 reflectDir = reflect(-lightDir, norm);
    
    //Calculate specular strength
    //Last parameter is shininess, specular highlight
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    //Add colour
    vec3 specular = spec * vec3(1, 1, 1);


    //Texture read
    vec4 tex = texture2D(u_Texture, v_TexCoord);
    
    //Apply lighting
    light = ambient;
    light += diff;
    light += specular;
    gl_FragColor = vec4(light, u_dissolve) * tex;
}