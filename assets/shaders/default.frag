#version 450 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vWorldPos;

uniform sampler2D uTexture;
uniform vec3 uLightDir = vec3(0.5, 1.0, 0.3);
uniform vec3 uLightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 uAmbient = vec3(0.1, 0.1, 0.1);

out vec4 FragColor;

void main() {
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(-uLightDir);
    
    // Simple Lambertian diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    vec3 color = texture(uTexture, vUV).rgb;
    vec3 finalColor = (uAmbient + diffuse) * color;
    
    FragColor = vec4(finalColor, 1.0);
}

