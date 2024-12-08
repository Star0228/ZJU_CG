#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct Torus_Vertex {
    float Position[3];
    float Normal[3];
    float TexCoords[2];
};

std::vector<Torus_Vertex> GenerateTorusVertices(float innerRadius, float outerRadius, int sides, int rings) {
    std::vector<Torus_Vertex> vertices;
    float ringFactor = (2.0f * PI) / rings;
    float sideFactor = (2.0f * PI) / sides;

    for (int ring = 0; ring <= rings; ++ring) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);

        for (int side = 0; side <= sides; ++side) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);

            float x = (outerRadius + innerRadius * cv) * cu;
            float y = (outerRadius + innerRadius * cv) * su;
            float z = innerRadius * sv;

            Torus_Vertex Torus_vertex;
            Torus_vertex.Position[0] = x;
            Torus_vertex.Position[1] = y;
            Torus_vertex.Position[2] = z;

            Torus_vertex.Normal[0] = cv * cu;
            Torus_vertex.Normal[1] = cv * su;
            Torus_vertex.Normal[2] = sv;

            Torus_vertex.TexCoords[0] = u / (2.0f * PI);
            Torus_vertex.TexCoords[1] = v / (2.0f * PI);

            vertices.push_back(Torus_vertex);
        }
    }

    return vertices;
}

std::vector<unsigned int> GenerateTorusIndices(int sides, int rings) {
    std::vector<unsigned int> indices;

    for (int ring = 0; ring < rings; ++ring) {
        for (int side = 0; side < sides; ++side) {
            int first = (ring * (sides + 1)) + side;
            int second = first + sides + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    return indices;
}

class Torus {
private:
    unsigned int VAO,VBO,EBO;
    std::vector<Torus_Vertex> mVertices;
    std::vector<unsigned int> mIndices;
public:
    Torus(float innerRadius, float outerRadius, int sides, int rings) {
        mVertices = GenerateTorusVertices(innerRadius, outerRadius, sides, rings);
        mIndices = GenerateTorusIndices(sides, rings);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Torus_Vertex), &mVertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

        // 设置顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Torus_Vertex), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Torus_Vertex), (void*)offsetof(Torus_Vertex, Normal));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Torus_Vertex), (void*)offsetof(Torus_Vertex, TexCoords));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        
    }
    void draw() {
        // draw the torus
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};