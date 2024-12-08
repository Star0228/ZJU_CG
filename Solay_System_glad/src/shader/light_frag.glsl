
#version 330 core
out vec4 FragColor;
in vec2 outTexCoord;

uniform vec3 lightColor;
uniform sampler2D diffuse; // 漫反射贴图
uniform sampler2D specular; // 镜面光贴图
void main() {

  vec3 temp = vec3(texture(diffuse, outTexCoord));
  temp += vec3(texture(specular, outTexCoord));
  // FragColor = vec4(temp, 1.0);
  FragColor = vec4(vec3(1.0f,1.0f,1.0f) * temp, 1.0);
}