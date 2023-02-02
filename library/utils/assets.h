#pragma once
#include "pch.h"
#include "graphics/mesh.h"

namespace prncss
{
    class assets
    {
    public:
        struct prefab
        {
            std::vector<mesh::data> meshes;
        };
    public:
        static std::string load_file(const char* path)
        {
            std::ifstream file;
            file.open(path);
            std::stringstream text_stream;
            text_stream << file.rdbuf();
            file.close();
            std::string text = text_stream.str();
            return text;
        }
        static prefab load_prefab(const char* path)
        {
            prefab result;

            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);
            process_node(scene->mRootNode, scene, &result);
            
            return result;
        }
    private:
        static void process_node(aiNode* node, const aiScene* scene, prefab* result)
        {
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
                result->meshes.push_back(process_mesh(scene->mMeshes[node->mMeshes[i]], scene));
                
            for(unsigned int i = 0; i < node->mNumChildren; i++)
                process_node(node->mChildren[i], scene, result);
        }
        static mesh::data process_mesh(aiMesh* a_mesh, const aiScene* scene)
        {
            mesh::data m;
            m.name = a_mesh->mName.C_Str();

            for (unsigned int i = 0; i < a_mesh->mNumVertices; i++)
            {
                mesh::vertex vertice;
                glm::vec3 v_pos;
                v_pos.x = a_mesh->mVertices[i].x;
                v_pos.y = a_mesh->mVertices[i].y;
                v_pos.z = a_mesh->mVertices[i].z;
                glm::vec3 v_nor;
                v_nor.x = a_mesh->mNormals[i].x;
                v_nor.y = a_mesh->mNormals[i].y;
                v_nor.z = a_mesh->mNormals[i].z;
                glm::vec2 v_uv;
                v_uv.x = a_mesh->mTextureCoords[0][i].x; 
                v_uv.y = a_mesh->mTextureCoords[0][i].y;
                vertice.position = v_pos;
                vertice.normal = v_nor;
                vertice.uv = v_uv;
                m.vertices.push_back(vertice);
            }

            /*for(unsigned int i = 0; i < a_mesh->mNumBones; i++)
            {
                int bone_id = -1;
                std::string bone_name = a_mesh->mBones[i]->mName.C_Str();
            }*/
            
            for(unsigned int i = 0; i < a_mesh->mNumFaces; i++)
            {
                aiFace face = a_mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    m.indices.push_back(face.mIndices[j]);
            }

            return m;
        }
    };
}