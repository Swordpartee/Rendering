#include "rendering/RenderObject.hpp"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace Rendering 
{
    RenderObject::RenderObject(const std::vector<Vertex>& vertices, 
                              const std::vector<unsigned int>& indices)
        : m_position(0.0f), m_rotation(0.0f), m_scale(1.0f), 
          m_transformDirty(true), m_textureID(0), m_color(1.0f, 1.0f, 1.0f)
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

    void RenderObject::render(const RenderContext& context)
    {
        // Default rendering implementation
        // Set model matrix for this object
        glm::mat4 model = getModelMatrix();
        glUniformMatrix4fv(context.modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Set object color
        glm::vec3 color = getColor();
        glUniform3fv(context.colorLoc, 1, glm::value_ptr(color));

        // Bind texture
        unsigned int textureToUse = getTexture();
        if (textureToUse == 0)
            textureToUse = context.defaultTexture;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureToUse);
        glUniform1i(context.textureLoc, 0);

        // Render the object
        glBindVertexArray(getVAO());
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void RenderObject::update(float deltaTime)
    {
        // Default update implementation - can be overridden by derived classes
        // Base objects don't need to do anything by default
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
}