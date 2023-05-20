#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "q_mesh.hpp"

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

namespace Helios
{
    class Model
    {
    public:
        // model data 
        vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        vector<Mesh>    meshes;
        string directory;
        bool gammaCorrection;

        // constructor, expects a filepath to a 3D model.
        Model(const std::string& path, bool gamma = false);

        // draws the model, and thus all its meshes
        //void Draw(Shader& shader);
        //void draw_depth(Shader& shader);

    private:
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(const std::string& path);

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}