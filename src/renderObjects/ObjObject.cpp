#include "../../include/rendering/RenderObject.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>

using namespace Rendering;

ObjObject::ObjObject(const std::string& objFilePath)
    : RenderObject(loadObjVertices(objFilePath), loadObjIndices(objFilePath)), 
      m_objFilePath(objFilePath)
{
    std::cout << "Loaded OBJ file: " << objFilePath << std::endl;
}

ObjObject::ObjObject(const std::string& objFilePath, unsigned int textureID)
    : RenderObject(loadObjVertices(objFilePath), loadObjIndices(objFilePath)), 
      m_objFilePath(objFilePath)
{
    setTexture(textureID);
    std::cout << "Loaded OBJ file: " << objFilePath << " with custom texture" << std::endl;
}

std::vector<Vertex> ObjObject::loadObjVertices(const std::string& filePath)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        // Return a simple triangle if file loading fails
        return {
            Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            Vertex(glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f))
        };
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            parseVertex(line, positions);
        }
        else if (prefix == "vt") {
            parseTexCoord(line, texCoords);
        }
        else if (prefix == "vn") {
            parseNormal(line, normals);
        }
        else if (prefix == "f") {
            parseFace(line, positions, texCoords, normals, vertices, indices);
        }
    }

    file.close();

    // If no normals were provided, calculate them
    if (!vertices.empty() && !indices.empty()) {
        bool hasNormals = false;
        for (const auto& vertex : vertices) {
            if (glm::length(vertex.normal) > 0.1f) {
                hasNormals = true;
                break;
            }
        }
        if (!hasNormals) {
            calculateNormals(vertices, indices);
        }
    }

    std::cout << "Loaded " << vertices.size() << " vertices from " << filePath << std::endl;
    return vertices;
}

std::vector<unsigned int> ObjObject::loadObjIndices(const std::string& filePath)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << filePath << std::endl;
        // Return indices for a simple triangle if file loading fails
        return {0, 1, 2};
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            parseVertex(line, positions);
        }
        else if (prefix == "vt") {
            parseTexCoord(line, texCoords);
        }
        else if (prefix == "vn") {
            parseNormal(line, normals);
        }
        else if (prefix == "f") {
            parseFace(line, positions, texCoords, normals, vertices, indices);
        }
    }

    file.close();
    return indices;
}

void ObjObject::parseVertex(const std::string& line, std::vector<glm::vec3>& positions)
{
    std::istringstream iss(line);
    std::string prefix;
    float x, y, z;
    
    iss >> prefix >> x >> y >> z;
    positions.push_back(glm::vec3(x, y, z));
}

void ObjObject::parseTexCoord(const std::string& line, std::vector<glm::vec2>& texCoords)
{
    std::istringstream iss(line);
    std::string prefix;
    float u, v;
    
    iss >> prefix >> u >> v;
    texCoords.push_back(glm::vec2(u, v));
}

void ObjObject::parseNormal(const std::string& line, std::vector<glm::vec3>& normals)
{
    std::istringstream iss(line);
    std::string prefix;
    float x, y, z;
    
    iss >> prefix >> x >> y >> z;
    normals.push_back(glm::vec3(x, y, z));
}

void ObjObject::parseFace(const std::string& line, 
                         const std::vector<glm::vec3>& positions,
                         const std::vector<glm::vec2>& texCoords,
                         const std::vector<glm::vec3>& normals,
                         std::vector<Vertex>& vertices,
                         std::vector<unsigned int>& indices)
{
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix; // Skip "f"

    std::string vertexData;
    std::vector<std::string> faceVertices;
    
    // Read all vertex data for this face
    while (iss >> vertexData) {
        faceVertices.push_back(vertexData);
    }

    // Process each vertex in the face
    std::vector<unsigned int> faceIndices;
    for (const std::string& vertData : faceVertices) {
        // Parse vertex/texture/normal indices (format: v/vt/vn or v//vn or v/vt or v)
        std::vector<int> vertIndices = parseVertexData(vertData);
        
        glm::vec3 position(0.0f);
        glm::vec2 texCoord(0.0f);
        glm::vec3 normal(0.0f, 1.0f, 0.0f);
        
        if (!vertIndices.empty() && vertIndices[0] > 0 && vertIndices[0] <= positions.size()) {
            position = positions[vertIndices[0] - 1]; // OBJ indices are 1-based
        }
        
        if (vertIndices.size() > 1 && vertIndices[1] > 0 && vertIndices[1] <= texCoords.size()) {
            texCoord = texCoords[vertIndices[1] - 1];
        }
        
        if (vertIndices.size() > 2 && vertIndices[2] > 0 && vertIndices[2] <= normals.size()) {
            normal = normals[vertIndices[2] - 1];
        }

        Vertex vertex(position, texCoord, normal);

        // Check if we already have this exact vertex
        unsigned int vertexIndex = findOrAddVertex(vertex, vertices);
        faceIndices.push_back(vertexIndex);
    }

    // Triangulate the face (assuming it's a polygon)
    for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
        indices.push_back(faceIndices[0]);
        indices.push_back(faceIndices[i]);
        indices.push_back(faceIndices[i + 1]);
    }
}

std::vector<int> ObjObject::parseVertexData(const std::string& vertexData)
{
    std::vector<int> result;
    std::istringstream iss(vertexData);
    std::string indexStr;
    
    while (std::getline(iss, indexStr, '/')) {
        if (indexStr.empty()) {
            result.push_back(0);
        } else {
            result.push_back(std::stoi(indexStr));
        }
    }
    
    return result;
}

unsigned int ObjObject::findOrAddVertex(const Vertex& vertex, std::vector<Vertex>& vertices)
{
    // Simple linear search - could be optimized with a hash map for large models
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (verticesEqual(vertices[i], vertex)) {
            return static_cast<unsigned int>(i);
        }
    }
    
    vertices.push_back(vertex);
    return static_cast<unsigned int>(vertices.size() - 1);
}

bool ObjObject::verticesEqual(const Vertex& a, const Vertex& b)
{
    const float epsilon = 0.0001f;
    return (glm::length(a.position - b.position) < epsilon &&
            glm::length(a.texCoord - b.texCoord) < epsilon &&
            glm::length(a.normal - b.normal) < epsilon);
}

void ObjObject::calculateNormals(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    // Reset all normals to zero
    for (auto& vertex : vertices) {
        vertex.normal = glm::vec3(0.0f);
    }

    // Calculate face normals and accumulate vertex normals
    for (size_t i = 0; i < indices.size(); i += 3) {
        if (i + 2 < indices.size()) {
            unsigned int i0 = indices[i];
            unsigned int i1 = indices[i + 1];
            unsigned int i2 = indices[i + 2];

            if (i0 < vertices.size() && i1 < vertices.size() && i2 < vertices.size()) {
                glm::vec3 v1 = vertices[i1].position - vertices[i0].position;
                glm::vec3 v2 = vertices[i2].position - vertices[i0].position;
                glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

                vertices[i0].normal += normal;
                vertices[i1].normal += normal;
                vertices[i2].normal += normal;
            }
        }
    }

    // Normalize all vertex normals
    for (auto& vertex : vertices) {
        if (glm::length(vertex.normal) > 0.0f) {
            vertex.normal = glm::normalize(vertex.normal);
        } else {
            vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f); // Default up normal
        }
    }
}