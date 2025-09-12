#version 330 core
out vec4 FragColor;
uniform sampler2D texture1;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / vec2(textureSize(texture1, 0));
    FragColor = texture(texture1, texCoord);
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}