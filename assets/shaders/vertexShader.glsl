#version 330 core
layout(location = 0) in vec3 position;  // 3D position
layout(location = 1) in vec2 texCoord;  // Texture coordinates
layout(location = 2) in vec3 normal;    // Normal vector (for future lighting)

out vec2 TexCoord;  // Pass texture coordinates to fragment shader

uniform mat4 model;       // Model transformation matrix
uniform mat4 view;        // View/camera matrix  
uniform mat4 projection;  // Projection matrix

void main()
{
    TexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}