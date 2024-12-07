#ifndef MYBEZIER_H
#define MYBEZIER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tool/shader.h>

struct Bezier_Vertex
{
    glm::vec3 Position; // 顶点属性
    glm::vec4 Color;    // 颜色
};
class MyBezier
{
private:
    int _numGrid;
    int _num_ControlPoints = 4;
    glm::vec4 _color;
    std::vector<glm::vec3> controlPoints;
    std::vector<Bezier_Vertex> surfacePoints;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
    float height = 0.2f;
public:
    MyBezier(int numGrid, glm::vec4 color = glm::vec4(0.97, 0.67, 0.02, 0.8))
        : _numGrid(numGrid), _color(color)
    {
        // 初始化控制点
        controlPoints = {
            glm::vec3(-1.0f, -3.6f, -1.0f + height),
            glm::vec3(-0.3f, -2.3f, -1.0f + height),
            glm::vec3(0.3f, -2.3f, -1.0f + height),
            glm::vec3(1.0f, -3.6f, -1.0f + height),

            glm::vec3(-1.8f, -2.0f, 0.0f + height),
            glm::vec3(-1.2f, -2.0f, 0.1f + height),
            glm::vec3(1.2f, -2.0f, 0.1f + height),
            glm::vec3(1.8f, -2.0f, 0.0f + height),

            glm::vec3(-2.5f, -1.0f, 0.0f + height),
            glm::vec3(-1.0f, -1.0f, 0.2f + height),
            glm::vec3(1.0f, -1.0f, 0.2f + height),
            glm::vec3(2.5f, -1.0f, 0.0f + height),

            glm::vec3(-2.5f, 0.5f, 0.0f + height),
            glm::vec3(-1.5f, 0.3f, 0.2f + height),
            glm::vec3(1.5f, 0.3f, 0.2f + height),
            glm::vec3(2.5f, 0.5f, 0.0f + height)};
    }

    ~MyBezier()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void Init()
    {
        // 生成曲面点
        GenerateSurface();
        // 创建 VAO 和 VBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, surfacePoints.size() * sizeof(Bezier_Vertex), &surfacePoints[0], GL_STATIC_DRAW);

        // 设置顶点属性指针
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Bezier_Vertex), (void *)0);

        // 设置颜色属性指针
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Bezier_Vertex), (void *)offsetof(Bezier_Vertex, Color));
        // 设置索引
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw(Shader &shader)
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void GenerateSurface()
    {
        surfacePoints.clear();
        for (int i = 0; i <= _numGrid; ++i)
        {
            for (int j = 0; j <= _numGrid; ++j)
            {
                float u = static_cast<float>(i) / _numGrid;
                float v = static_cast<float>(j) / _numGrid;
                glm::vec3 point = BezierPoint(u, v);
                Bezier_Vertex vertex;
                vertex.Position = point;
                vertex.Color = _color;
                surfacePoints.push_back(vertex);
                // cout << "point: " << point.x << " " << point.y << " " << point.z << endl;
                // cout << "color: " << color.x << " " << color.y << " " << color.z << endl;
            }
        }
        for (int i = 0; i < _numGrid; ++i)
        {
            for (int j = 0; j < _numGrid; ++j)
            {
                int seq = i * (_numGrid + 1) + j;
                // No.1 triangle
                indices.push_back(seq);
                indices.push_back(seq + 1);
                indices.push_back(seq + _numGrid + 1);
                // No.2 triangle
                indices.push_back(seq + 1);
                indices.push_back(seq + _numGrid + 2);
                indices.push_back(seq + _numGrid + 1);
            }
        }
    }
    glm::vec3 BezierPoint(float u, float v)
    {
        glm::vec3 point(0.0f);
        for (int i = 0; i < _num_ControlPoints; ++i)
        {
            for (int j = 0; j < _num_ControlPoints; ++j)
            {
                float basisU = BernsteinBasis(i, u);
                float basisV = BernsteinBasis(j, v);
                point += controlPoints[i * _num_ControlPoints + j] * basisU * basisV;
            }
        }
        return point;
    }

    float BernsteinBasis(int i, float t)
    {
        if (i < 0 || i > 3)
            return 0.0f;
        if (i == 0)
            return pow(1 - t, 3);
        if (i == 1)
            return 3 * t * pow(1 - t, 2);
        if (i == 2)
            return 3 * pow(t, 2) * (1 - t);
        if (i == 3)
            return pow(t, 3);
    }
};

#endif