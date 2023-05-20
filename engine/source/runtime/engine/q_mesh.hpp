#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
//#include "q_shader.hpp"


namespace Helios
{
    using namespace std;
    #define MAX_BONE_INFLUENCE 4
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        // mesh Data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;
        unsigned int VAO;

        // constructor
        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

        // render the mesh
        //void Draw(Shader& shader);
        //void draw_depth(Shader& shader);

    private:
        // render data 
        unsigned int VBO, EBO;

        // initializes all the buffer objects/arrays
        void setupMesh();
    };
}