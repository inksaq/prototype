#version 410 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform int useTexture;

void main()
{
    if(useTexture == 1) {
        vec4 texColor = texture(texture1, TexCoord);
        if(texColor.a < 0.1)
            discard;
        FragColor = texColor;
    } else {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
}