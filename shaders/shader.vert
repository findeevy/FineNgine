#version 450

layout(binding = 0) uniform UniformBufferObject {
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPosition;   
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragPos;
layout(location = 3) out vec3 fragNormal;

void main() {
  //Calculate the world position of our vertex.
  vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
  //Calculate the position on our screen.
  gl_Position = ubo.proj * ubo.view * worldPosition;
  fragPos = worldPosition.xyz;
  //Move off of GPU later, precompute? Moves normals from model space into worldspace.
  fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
  //Pass in values.
  fragColor = inColor;
  fragTexCoord = inTexCoord;
}
