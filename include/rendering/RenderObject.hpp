#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include "Collider.hpp"

namespace Rendering 
{
    // Forward declarations
    class ParteeCamera;
    
    // Rendering context to pass shared state to objects
    struct RenderContext 
    {
        unsigned int shaderProgram;
        int modelLoc;
        int viewLoc;
        int projectionLoc;
        int textureLoc;
        int colorLoc;
        unsigned int defaultTexture;
        const ParteeCamera* camera;
        float totalTime;
        float deltaTime;
    };

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
        void setLinearVelocity(const glm::vec3 &linearVelocity);
        void setAngularVelocity(const glm::vec3 &angularVelocity);

        void applyLinearForce(const glm::vec3 &linearImpulse);
        void applyAngulerForce(const glm::vec3 &angularImpulse);

        void translate(const glm::vec3 &translation);
        void rotate(float angle, const glm::vec3& axis);
        void scale(const glm::vec3& scale);

        // Matrix getters
        glm::mat4 getModelMatrix() const;
        
        // Rendering - virtual method for object-specific rendering
        virtual void render(const RenderContext& context);
        virtual void update(float deltaTime);

        void updatePhysics(float deltaTime);

        // Rendering data
        unsigned int getVAO() const { return m_VAO; }
        unsigned int getIndexCount() const { return m_indexCount; }
        
        // Material properties (for future use)
        void setTexture(unsigned int textureID) { m_textureID = textureID; }
        unsigned int getTexture() const { return m_textureID; }
        
        // Color properties
        void setColor(const glm::vec3& color) { m_color = color; }
        glm::vec3 getColor() const { return m_color; }

    protected:
        // Transform components
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;
        glm::vec3 m_linearVelocity;
        glm::vec3 m_angularVelocity;

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
        glm::vec3 m_color;

        void updateModelMatrix() const;
        void setupMesh(const std::vector<Vertex>& vertices, 
                      const std::vector<unsigned int>& indices);
    };

    // Predefined shapes
    class CubeObject : public RenderObject 
    {
    public:
        CubeObject();
        void render(const RenderContext& context) override;
        static std::vector<Vertex> getCubeVertices();
        static std::vector<unsigned int> getCubeIndices();
    };

    class SphereObject : public RenderObject 
    {
    public:
        SphereObject(int latitudeSegments = 32, int longitudeSegments = 32, float radius = 1.0f);
        void render(const RenderContext& context) override;
        void update(float deltaTime) override;
        
        // Collision methods
        SphereCollider& getCollider() { return m_collider; }
        const SphereCollider& getCollider() const { return m_collider; }
        void updateColliderPosition();
        
        // Color management for collision indication
        void setColliding(bool isColliding);
        
        static std::vector<Vertex> getSphereVertices(int latitudeSegments = 32, int longitudeSegments = 32);
        static std::vector<unsigned int> getSphereIndices(int latitudeSegments = 32, int longitudeSegments = 32);
        
    private:
        SphereCollider m_collider;
        float m_sphereRadius;
        bool m_isColliding;
    };

    class ObjObject : public RenderObject 
    {
    public:
        ObjObject(const std::string& objFilePath);
        ObjObject(const std::string& objFilePath, unsigned int textureID);
        
        static std::vector<Vertex> loadObjVertices(const std::string& filePath);
        static std::vector<unsigned int> loadObjIndices(const std::string& filePath);
        
    private:
        std::string m_objFilePath;
        
        // Helper functions for parsing OBJ format
        static void parseVertex(const std::string& line, std::vector<glm::vec3>& positions);
        static void parseTexCoord(const std::string& line, std::vector<glm::vec2>& texCoords);
        static void parseNormal(const std::string& line, std::vector<glm::vec3>& normals);
        static void parseFace(const std::string& line, 
                             const std::vector<glm::vec3>& positions,
                             const std::vector<glm::vec2>& texCoords,
                             const std::vector<glm::vec3>& normals,
                             std::vector<Vertex>& vertices,
                             std::vector<unsigned int>& indices);
        static std::vector<int> parseVertexData(const std::string& vertexData);
        static unsigned int findOrAddVertex(const Vertex& vertex, std::vector<Vertex>& vertices);
        static bool verticesEqual(const Vertex& a, const Vertex& b);
        static void calculateNormals(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    };
}