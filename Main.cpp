#include "raylib.h"   // Raylib core library
#include "rlgl.h"     // For low-level rendering
#include <vector>     // Standard library for vectors
#include <cmath>      // For sin and cos functions

using namespace std;

int main(void)
{
    // Initialization
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    // Lorenz Attractor variables
    //x,y,z basically determine the starting point
    float x = 1.0f, y = 1.0f, z = 1.0f;
    const double alpha = 10.0;    // Sigma 10
    const double beta = 8.0/3.0; // Beta 8/3
    const double pro = 28.0;      // Rho 28
    const double dt = 0.01;       // Time step

    InitWindow(screenWidth, screenHeight, "Rotating Camera - Lorenz Attractor");

    // Camera Setup
    Camera camera = { 0 };
    float radius = 60.0f;  // Distance of the camera from the attractor point
    float angle = 0.0f;    // Rotation angle around the Z-axis
    camera.up = { 0.0f, 1.0f, 0.0f };          // Up direction
    camera.fovy = 30.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    // Vector to store Lorenz Attractor points
    vector<Vector3> points;

    SetTargetFPS(60);
    float hue = 0.0f; // For color progression

    // Main Game Loop
    while (!WindowShouldClose())
    {
        // Lorenz Attractor Update Equations
        double dx = (alpha * (y - x)) * dt;
        double dy = (x * (pro - z) - y) * dt;
        double dz = (x * y - beta * z) * dt;

        x += dx;
        y += dy;
        z += dz;

        points.push_back({ x, y, z });

        // Performance optimization: Limit the points vector size
        if (points.size() > 5000) points.erase(points.begin());

        // Camera position rotates around the Z-axis
        angle += 0.01f; // Increment the rotation angle
        camera.position.x = radius * cos(angle);
        camera.position.y = radius * sin(angle);
        camera.position.z = 40.0f; // Fixed height on Z-axis

        // Lock camera target to the latest Lorenz point
        camera.target = { x, y, z };

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        rlPushMatrix();
        rlSetLineWidth(3.0f);      // Set line thickness

        if (points.size() > 1)
        {
            for (size_t i = 0; i < points.size() - 1; i++)
            {
                Color n_color = ColorFromHSV(fmod(hue + i * 0.1f, 360.0f), 1.0f, 1.0f);
                DrawLine3D(points[i], points[i + 1], n_color);

            }
        }

        rlSetLineWidth(1.0f);      // Reset to default thickness
        rlPopMatrix();            
        EndMode3D();

        DrawText("3D Lorenz Attractor - Rotating Camera", 10, 20, 20, RAYWHITE);
        DrawFPS(50, 60);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
