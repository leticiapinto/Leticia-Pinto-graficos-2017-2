#pragma once

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Mesh.h"

using namespace std;

namespace graphics
{

    class Model 
    {
    public:

        vector<Mesh* > meshes;
        bool gammaCorrection;

        /*  Functions   */
        // constructor, expects a filepath to a 3D model.
        Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
        {
            loadModel(path);
        }

    private:
        /*  Functions   */
        // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
        void loadModel(string const &path)
        {
            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            // check for errors
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
                return;
            }

            // process ASSIMP's root node recursively
            processNode(scene->mRootNode, scene);
        }

        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode *node, const aiScene *scene)
        {
            // process each mesh located at the current node
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }

        }

        Mesh* processMesh(aiMesh *mesh, const aiScene *scene)
        {
            Mesh* _mesh = NULL;
            // data to fill
            vector<vec3> vertices;
            vector<Ind3> indices;
            vector<vec3> normals;

            // Walk through each of the mesh's vertices
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                vec3 v;
                // positions
                v.x = mesh->mVertices[i].x;
                v.y = mesh->mVertices[i].y;
                v.z = mesh->mVertices[i].z;
                
                vertices.push_back(v);

                // normals
                v.x = mesh->mNormals[i].x;
                v.y = mesh->mNormals[i].y;
                v.z = mesh->mNormals[i].z;
                normals.push_back(v);

            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                
                Ind3 indice;

                for(unsigned int j = 0; j < face.mNumIndices; j++)
                {
                    indice.buff[j] = face.mIndices[j];
                }
                indices.push_back(indice);
            }
           
            _mesh = new Mesh( vertices, normals, indices );

            return _mesh;
        }

    };



}