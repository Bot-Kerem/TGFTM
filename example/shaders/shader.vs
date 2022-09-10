#version 330 compatibility

layout (location = 0) in vec3 inPosition;

uniform mat4 view;
uniform mat4 proj;

uniform vec3 pos;

void main(){
    gl_Position = proj * view * vec4(inPosition - pos, 1.0f);
    
    //gl_Position = vec4(inPosition, 0, 1.0f);
    //h = inPosition.y;
}