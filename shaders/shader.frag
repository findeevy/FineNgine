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
  float lightRadius;
} lit;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragNormal;

layout(location = 0) out vec4 outColor;

void main() {
  // Re-normalize interpolated normal
  vec3 normal = normalize(fragNormal);
  
  // Calculate light direction (from fragment to light)
  vec3 lightDir = normalize(lit.lightPosition - fragPos);
  
  // Calculate view direction (from fragment to camera)
  vec3 viewDir = normalize(ubo.viewPosition - fragPos);
  
  // Calculate halfway vector for Blinn-Phong
  vec3 halfwayDir = normalize(lightDir + viewDir); 
  
  // Calculate distance from light to fragment
  float distance = length(lit.lightPosition - fragPos);
  
  // lght intensity falls off to zero at lightRadius
  float attenuation = clamp(1.0 - (distance * distance) / (lit.lightRadius * lit.lightRadius), 0.0, 1.0);
  attenuation = attenuation * attenuation;
  
  // Ambient lighting
  float ambientStrength = 0.01;
  vec3 ambient = ambientStrength * lit.lightColor;

  // Diffuse lighting (Lambertian reflection)
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lit.lightColor;

  // Specular lighting (Blinn-Phong)
  float specularStrength = 1.0;
  float shininess = 64.0;
  float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * lit.lightColor;

  // Apply attenuation to direct lighting components
  diffuse *= attenuation;
  specular *= attenuation;

  // Combine all lighting with texture
  vec3 texColor = texture(texSampler, fragTexCoord).rgb;
  vec3 result = (ambient + diffuse + specular) * texColor;
  
  outColor = vec4(result, 1.0);
}
