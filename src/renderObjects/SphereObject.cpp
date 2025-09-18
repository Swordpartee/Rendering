#include "../../include/rendering/RenderObject.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

using namespace Rendering;

// SphereObject implementation
SphereObject::SphereObject(int latitudeSegments, int longitudeSegments)
    : RenderObject(getSphereVertices(latitudeSegments, longitudeSegments),
                   getSphereIndices(latitudeSegments, longitudeSegments))
{
}

std::vector<Vertex> SphereObject::getSphereVertices(int latitudeSegments, int longitudeSegments)
{
    std::vector<Vertex> vertices;

    // Generate vertices
    for (int lat = 0; lat <= latitudeSegments; ++lat)
    {
        float theta = lat * glm::pi<float>() / latitudeSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int lon = 0; lon <= longitudeSegments; ++lon)
        {
            float phi = lon * 2 * glm::pi<float>() / longitudeSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            glm::vec3 position(x, y, z);
            glm::vec3 normal(x, y, z); // For a unit sphere, position = normal
            glm::vec2 texCoord((float)lon / longitudeSegments, (float)lat / latitudeSegments);

            vertices.emplace_back(position, texCoord, normal);
        }
    }

    return vertices;
}

std::vector<unsigned int> SphereObject::getSphereIndices(int latitudeSegments, int longitudeSegments)
{
    std::vector<unsigned int> indices;

    for (int lat = 0; lat < latitudeSegments; ++lat)
    {
        for (int lon = 0; lon < longitudeSegments; ++lon)
        {
            int first = lat * (longitudeSegments + 1) + lon;
            int second = first + longitudeSegments + 1;

            // First triangle
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Second triangle
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return indices;
}

// SphereObject specific rendering
void SphereObject::render(const RenderContext &context)
{
    // Spheres use standard rendering but with pulsing effect
    glm::vec3 baseColor = getColor();

    // Create unique phase offset based on object's position to avoid synchronization
    float phaseOffset = (getModelMatrix()[3][0] + getModelMatrix()[3][1] + getModelMatrix()[3][2]) * 0.1f;

    // Add time-based pulsing effect using proper time from context
    float pulse = 0.8f + 0.2f * sin(context.totalTime * 2.0f + phaseOffset);
    glm::vec3 pulsingColor = baseColor * pulse;

    // Set model matrix
    glm::mat4 model = getModelMatrix();
    glUniformMatrix4fv(context.modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    // Set pulsing color
    glUniform3fv(context.colorLoc, 1, glm::value_ptr(pulsingColor));

    // Bind texture
    unsigned int textureToUse = getTexture();
    if (textureToUse == 0)
        textureToUse = context.defaultTexture;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureToUse);
    glUniform1i(context.textureLoc, 0);

    // Render sphere
    glBindVertexArray(getVAO());
    glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}