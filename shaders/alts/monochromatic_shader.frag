#version 450

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPosition;         
} ubo;

layout(binding = 1) uniform LightingBufferObject {
  vec3 lightPosition;
  vec3 lightColor;
} lit;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

void main() {
  //Normalization of our data.
  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(lit.lightPosition - fragPos);
  vec3 viewDir = normalize(ubo.viewPosition - fragPos);
  vec3 halfwayDir = normalize(lightDir + viewDir); 
   
  //Set up ambient lighting.
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lit.lightColor;

  //Set up diffuse lighting.
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lit.lightColor;

  //Set up specular lighting.
  float specularStrength = 0.5;
  float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lit.lightColor;

  //Merge all of our lighting calculations.
  vec3 result = (ambient + diffuse + specular) * texture(texSampler, fragTexCoord).rgb;
  float bw = (result.x+result.y+result.z)/3;
  outColor = vec4(bw, bw, bw, 1.0);
}
