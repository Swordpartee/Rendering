#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

namespace Rendering 
{
    struct Vertex 
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;  // For future lighting

        Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& norm = glm::vec3(0.0f, 0.0f, 1.0f))
            : position(pos), texCoord(tex), normal(norm) {}
    };

    class RenderObject 
    {
    public:
        RenderObject(const std::vector<Vertex>& vertices, 
                    const std::vector<unsigned int>& indices);
        virtual ~RenderObject();

        // Transform operations
        void setPosition(const glm::vec3& position);
        void setRotation(const glm::vec3& rotation);
        void setScale(const glm::vec3& scale);
        
        void translate(const glm::vec3& translation);
        void rotate(float angle, const glm::vec3& axis);
        void scale(const glm::vec3& scale);

        // Matrix getters
        glm::mat4 getModelMatrix() const;
        
        // Rendering data
        unsigned int getVAO() const { return m_VAO; }
        unsigned int getIndexCount() const { return m_indexCount; }
        
        // Material properties (for future use)
        void setTexture(unsigned int textureID) { m_textureID = textureID; }
        unsigned int getTexture() const { return m_textureID; }

    protected:
        // Transform components
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        
        // Cached model matrix
        mutable glm::mat4 m_modelMatrix;
        mutable bool m_transformDirty;

        // OpenGL objects
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_EBO;
        unsigned int m_indexCount;
        
        // Material
        unsigned int m_textureID;

        void updateModelMatrix() const;
        void setupMesh(const std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices);
    };

    // Predefined shapes
    class CubeObject : public RenderObject 
    {
    public:
        CubeObject();
        static std::vector<Vertex> getCubeVertices();
        static std::vector<unsigned int> getCubeIndices();
    };
}