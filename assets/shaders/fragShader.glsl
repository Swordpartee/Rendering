#version 330 core
in vec2 TexCoord;         // Receive from vertex shader
out vec4 FragColor;       // Output color

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoord);
}