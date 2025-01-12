#version 460 core
out vec4 FragColor;

uniform vec4 uGridColor;
uniform float uFadeDistance;

void main() {
    float dist = length(gl_FragCoord.xy / gl_FragCoord.w);
    float alpha = 1.0 - (dist / uFadeDistance);
    FragColor = vec4(uGridColor.rgb, uGridColor.a * alpha);
}