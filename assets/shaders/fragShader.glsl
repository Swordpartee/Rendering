#version 330 core
in vec2 TexCoord;         // Receive from vertex shader
out vec4 FragColor;       // Output color

uniform sampler2D texture1;
uniform vec3 objectColor; // Object color uniform

void main()
{
    vec4 textureColor = texture(texture1, TexCoord);
    // Mix texture with object color
    FragColor = vec4(objectColor, 1.0) * textureColor;
}