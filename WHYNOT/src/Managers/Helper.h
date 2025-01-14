#pragma once
#include <includes.h>


class Helper
{
    static GLFWwindow* window;
    
public:
    static float windowWidth;
    static float windowHeight;
    
    static void InitializeGlfw();
    static void InitializeGlad();
    static GLFWwindow* GetWindow();
    static void EndUpdate(float deltaTime);
    static void Draw();
    static void Terminate();

    // Generate sphere vertices
    static void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks) {
        const float PI = 3.14159265359f;

        // Vertices
        for (int stack = 0; stack <= stacks; ++stack) {
            float stackAngle = PI / 2 - stack * PI / stacks; // From PI/2 to -PI/2
            float xy = radius * cosf(stackAngle);           // r * cos(phi)
            float z = radius * sinf(stackAngle);            // r * sin(phi)

            for (int sector = 0; sector <= sectors; ++sector) {
                float sectorAngle = 2 * PI * sector / sectors; // From 0 to 2PI

                // Vertex position (x, y, z)
                float x = xy * cosf(sectorAngle); // r * cos(phi) * cos(theta)
                float y = xy * sinf(sectorAngle); // r * cos(phi) * sin(theta)
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // Normalized normals (x, y, z)
                vertices.push_back(x / radius);
                vertices.push_back(y / radius);
                vertices.push_back(z / radius);

                // Texture coordinates (u, v)
                float u = (float)sector / sectors;
                float v = (float)stack / stacks;
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }

        // Indices
        for (int stack = 0; stack < stacks; ++stack) {
            for (int sector = 0; sector < sectors; ++sector) {
                int first = stack * (sectors + 1) + sector;
                int second = first + sectors + 1;

                // Triangle 1
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                // Triangle 2
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }

};
