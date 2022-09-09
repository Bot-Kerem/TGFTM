#version 330 compatibility

out vec4 fragColor;

uniform vec3 color;

void main(){
    //fragColor = vec4(texture(tex, uv).rgb,1);
    fragColor = vec4(0.52,0.52, 0.52, 1);
}