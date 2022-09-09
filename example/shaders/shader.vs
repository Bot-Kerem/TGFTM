#version 330 compatibility

layout (location = 0) in vec3 inPosition;

uniform mat4 view;
uniform mat4 proj;

void main(){
    gl_Position = proj * view * vec4(inPosition, 1.0f);
}