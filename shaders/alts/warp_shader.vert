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

float chunkiness = 30.0;

void main() {
  vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
  vec4 clipPos = ubo.proj * ubo.view * worldPosition;

  vec2 ndcPos = clipPos.xy / clipPos.w;

  float gridSize = 1.0 / 30.0;

  vec2 snappedNDC = floor(ndcPos / gridSize) * gridSize + gridSize * 0.5;

  clipPos.xy = snappedNDC * clipPos.w;

  gl_Position = clipPos;

  fragPos = worldPosition.xyz;
  fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
  fragColor = inColor;
  fragTexCoord = inTexCoord;
}
