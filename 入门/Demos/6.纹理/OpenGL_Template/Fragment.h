#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in float Img;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform sampler2D avatarTexture;
uniform float factor;
void main()
{
    if (Img == 1.0) {
        FragColor = mix(texture(ourTexture, TexCoord),texture(avatarTexture, TexCoord),factor) * vec4(ourColor, 1.0);
    } else {
        FragColor = vec4(ourColor, 1.0);
    }
}

