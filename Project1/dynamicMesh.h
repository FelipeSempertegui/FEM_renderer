#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include<chrono>
#include<thread>

class dynamicMesh {

private:
    std::vector < std::vector<std::vector<float>> > dynMesh;
    std::vector < std::vector<std::vector<float>> > dynColors;
    std::vector < std::vector<std::vector<float>> > dynNormals;

    unsigned int vertexbuffer;
    unsigned int colorbuffer;
    unsigned int normalbuffer;

    int frames;
    int freqIndexMax;

    int freqIndex, counter;
    std::chrono::milliseconds delay;

    void initRendererVariables();

    void initDynamicVAO();

public:
    dynamicMesh();

    dynamicMesh(std::vector < std::vector<std::vector<float>> > dynMesh, std::vector < std::vector<std::vector<float>> >dynColors, std::vector < std::vector<std::vector<float>> >dynNormals, int frames, int freqIndexMax);

    ~dynamicMesh();

    void initializeMesh(std::vector < std::vector<std::vector<float>> > dynMesh, std::vector < std::vector<std::vector<float>> >dynColors, std::vector < std::vector<std::vector<float>> >dynNormals, int frames, int freqIndexMax);

    void drawCall();

    void selectCurrentTimeStep();

    void changeFrequency(GLFWwindow* window);

    void incrementFrames();

};
