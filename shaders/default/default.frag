#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform int useTexture; // 0 for color, 1 for texture

void main()
{
    if (useTexture == 1) {
        FragColor = texture(texture1, TexCoord);
    } else {
        // Render with a solid color if no texture is used
        FragColor = vec4(1.0, 0.5, 0.2, 1.0); // Example color: orange
    }
}
