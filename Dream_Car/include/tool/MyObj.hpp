#ifndef MyObj_HPP
#define MyObj_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <tool/shader.h>

#include <string>
#include <vector>
using namespace std;
struct Obj_Vertex
{
    glm::vec3 Position;  // 顶点属性
    glm::vec3 Normal;    // 法线
    glm::vec2 TexCoords; // 纹理坐标
    glm::vec4 Color;     // 颜色
};
class MyObj
{
public:
    vector<Obj_Vertex> vertex;
    glm::vec4 color;
    unsigned int VAO;
    unsigned int VBO;
    vector<float> _vert;
    vector<float> _tex;
    vector<float> _norm;
    // mesh Data
    MyObj(const char *path, glm::vec4 _color = glm::vec4(0.04, 0.9, 0.84, 0.88))
    {
        this->color = _color;
        loadModel(path);
        // printf("load model success\n");
        // printf("vertex size: %d\n", vertex.size());
    }
    // render the mesh
    void Draw(Shader &shader)
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertex.size()));
        glBindVertexArray(0);
    }
    void loadModel(const char *path)
    {
        float x = 0.f, y = 0.f, z = 0.f;
        string content;
        ifstream fileStream(path, ios::in);
        string str = "";

        while (!fileStream.eof())
        {
            getline(fileStream, str);
            if (str.compare(0, 2, "v ") == 0)
            {
                std::stringstream stream(str.erase(0, 1));
                stream >> x;
                _vert.push_back(x);
                stream >> y;
                _vert.push_back(y);
                stream >> z;
                _vert.push_back(z);
            }
            if (str.compare(0, 2, "vt") == 0)
            {
                std::stringstream stream(str.erase(0, 2));
                stream >> x;
                _tex.push_back(x);
                stream >> y;
                _tex.push_back(y);
            }
            if (str.compare(0, 2, "vn") == 0)
            {
                std::stringstream stream(str.erase(0, 2));
                stream >> x;
                _norm.push_back(x);
                stream >> y;
                _norm.push_back(y);
                stream >> z;
                _norm.push_back(z);
            }
            if (str.compare(0, 1, "f") == 0)
            {
                string oneCorner, v, t, n;
                std::stringstream stream(str.erase(0, 2));
                for (int i = 0; i < 3; i++)
                {
                    getline(stream, oneCorner, ' ');
                    stringstream oneCornerSS(oneCorner);
                    getline(oneCornerSS, v, '/');
                    getline(oneCornerSS, t, '/');
                    getline(oneCornerSS, n, '/');

                    int Vert_index = (stoi(v) - 1) * 3;
                    int Tex_index = (stoi(t) - 1) * 2;
                    int Normal_index = (stoi(n) - 1) * 3;

                    Obj_Vertex _new_vertex;
                    _new_vertex.Position = glm::vec3(_vert[Vert_index], _vert[Vert_index + 1], _vert[Vert_index + 2]);
                    _new_vertex.TexCoords = glm::vec2(_tex[Tex_index], _tex[Tex_index + 1]);
                    _new_vertex.Normal = glm::vec3(_norm[Normal_index], _norm[Normal_index + 1], _norm[Normal_index + 2]);
                    _new_vertex.Color = color;
                    vertex.push_back(_new_vertex);
                }
            }
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Obj_Vertex), &vertex[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Obj_Vertex), (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Obj_Vertex), (void *)offsetof(Obj_Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Obj_Vertex), (void *)offsetof(Obj_Vertex, TexCoords));
        // vertex color
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Obj_Vertex), (void *)offsetof(Obj_Vertex, Color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};

#endif