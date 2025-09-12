#include "rendering/RenderObject.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Rendering 
{
    RenderObject::RenderObject(const std::vector<Vertex>& vertices, 
                              const std::vector<unsigned int>& indices)
        : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), 
          m_transformDirty(true), m_textureID(0)
    {
        setupMesh(vertices, indices);
    }

    RenderObject::~RenderObject()
    {
        if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
        if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
        if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);
    }

    void RenderObject::setPosition(const glm::vec3& position)
    {
        m_position = position;
        m_transformDirty = true;
    }

    void RenderObject::setRotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        m_transformDirty = true;
    }

    void RenderObject::setScale(const glm::vec3& scale)
    {
        m_scale = scale;
        m_transformDirty = true;
    }

    void RenderObject::translate(const glm::vec3& translation)
    {
        m_position += translation;
        m_transformDirty = true;
    }

    void RenderObject::rotate(float angle, const glm::vec3& axis)
    {
        // This is a simplified rotation - for more complex rotations, 
        // you might want to use quaternions
        if (axis.x != 0.0f) m_rotation.x += angle;
        if (axis.y != 0.0f) m_rotation.y += angle;
        if (axis.z != 0.0f) m_rotation.z += angle;
        m_transformDirty = true;
    }

    void RenderObject::scale(const glm::vec3& scale)
    {
        m_scale *= scale;
        m_transformDirty = true;
    }

    glm::mat4 RenderObject::getModelMatrix() const
    {
        if (m_transformDirty)
        {
            updateModelMatrix();
        }
        return m_modelMatrix;
    }

    void RenderObject::updateModelMatrix() const
    {
        m_modelMatrix = glm::mat4(1.0f);
        
        // Apply transformations in order: Scale -> Rotate -> Translate
        m_modelMatrix = glm::translate(m_modelMatrix, m_position);
        
        // Apply rotations (XYZ order)
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        
        m_transformDirty = false;
    }

    void RenderObject::setupMesh(const std::vector<Vertex>& vertices, 
                                const std::vector<unsigned int>& indices)
    {
        m_indexCount = indices.size();

        // Generate buffers
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        // Bind VAO
        glBindVertexArray(m_VAO);

        // Load vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
                    vertices.data(), GL_STATIC_DRAW);

        // Load index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                    indices.data(), GL_STATIC_DRAW);

        // Position attribute (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Texture coordinate attribute (location 1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(1);

        // Normal attribute (location 2)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                             (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);

        // Unbind VAO
        glBindVertexArray(0);
    }

    // CubeObject implementation
    CubeObject::CubeObject() : RenderObject(getCubeVertices(), getCubeIndices())
    {
    }

    std::vector<Vertex> CubeObject::getCubeVertices()
    {
        return {
            // Front face
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            
            // Back face
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            
            // Left face
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f)),
            
            // Right face
            Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
            
            // Top face
            Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
            
            // Bottom face
            Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };
    }

    std::vector<unsigned int> CubeObject::getCubeIndices()
    {
        return {
            // Front face
            0, 1, 2,   2, 3, 0,
            // Back face
            4, 5, 6,   6, 7, 4,
            // Left face
            8, 9, 10,  10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 21, 22, 22, 23, 20
        };
    }
}