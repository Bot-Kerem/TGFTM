#include "../tgftm.h"
#include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include <glm/gtc/type_ptr.hpp>


std::string openFile(std::string path);

void loadMap(const char* mapName);

void scrollcb(GLFWwindow* window, double xoffset, double yoffset);
void cursorCallback(GLFWwindow *window, double InxPos, double InyPos);
void entercb(GLFWwindow* window, int entered);

inline bool exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

Camera camera;

bool isEntered = false;

bool isLoaded = false;

float x;
float y;
unsigned int vao = 0;
unsigned int vbo = 0;
int numVert;
bool process = false;

float lastX = 400;
float lastY = 300;

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Example", nullptr, nullptr);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetScrollCallback(window, scrollcb);
    glfwSetCursorEnterCallback(window, entercb);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    //************************************************************

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    //****************************************

    //****** SHADER

    unsigned int m_Program = glCreateProgram();

    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
    {
        std::string vertexSourceCode = openFile("./../shaders/shader.vs");
        const char* sourceCode = &vertexSourceCode[0];
        glShaderSource(VertexShader, 1, &sourceCode, 0);
        glCompileShader(VertexShader);
    }

    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        std::string fragmentSourceCode = openFile("./../shaders/shader.fs");
        const char* sourceCode = &fragmentSourceCode[0];
        glShaderSource(FragmentShader, 1, &sourceCode, 0);
        glCompileShader(FragmentShader);
    }

    
    glAttachShader(m_Program, VertexShader);
    glAttachShader(m_Program, FragmentShader);

    glLinkProgram(m_Program);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    //-***** SHADER

    unsigned int color = glGetUniformLocation(m_Program, "color");
    unsigned int pos = glGetUniformLocation(m_Program, "pos");
    unsigned int rot = glGetUniformLocation(m_Program, "rot");
    unsigned int scale = glGetUniformLocation(m_Program, "scale");
    unsigned int view = glGetUniformLocation(m_Program, "view");
    unsigned int proj = glGetUniformLocation(m_Program, "proj");

    camera.dis = 2.0f;

    glm::vec3 scalevec{1.0f, 0.4f, 1.0f};
    glm::vec3 rotvec{-1.0f, 1.0f, 1.0f};

    glClearColor(0.31f, 0.31f, 0.31f, 1.0f);
    char buf[50] = {'\0'};
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        if(isLoaded){
            glUseProgram(m_Program);

            glUniformMatrix4fv(view, 1, GL_FALSE, (float*)glm::value_ptr(camera.getView()));
            glUniformMatrix4fv(proj, 1, GL_FALSE, (float*)glm::value_ptr(camera.getPerspective()));
            glUniform3f(pos, x, 0, -y);
            glUniform3f(scale, scalevec.x, scalevec.y, scalevec.z);
            glUniform3f(rot, rotvec.x, rotvec.y, rotvec.z);

            glBindVertexArray(vao);
            glPolygonMode(GL_FRONT, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, numVert);
        }

        
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::Begin("Map")){
            
            ImGui::InputTextWithHint(" ", "Map Path", buf, 50);
            if(ImGui::Button("Load")){
                //if(exists_test0(buf)){
                    loadMap(std::string(std::string("./../maps/") + std::string(buf)).c_str());
                //}
                //else{
                //    std::cout << "File not exists:\n";
                //    std::cout << "File: " << buf << "\n";
                //}
            }
            ImGui::SliderFloat("  ", &(scalevec.y), 0, 1);
        }
        ImGui::End();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

std::string openFile(std::string path){
    std::ifstream file(path);
    if(!file.is_open()){
        std::cout << "File does not exists: " << path << std::endl;
        return std::string();
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void entercb(GLFWwindow* window, int entered){
    
    if(!entered &&process){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if(xpos <= 0){
            lastX = 800;
            xpos = 800 + xpos;
        }
        if(xpos >= 800){
            lastX = 0;
            xpos = xpos - 800;
        }
        if(ypos <= 0){
            lastY = 600;
            ypos = 600 + ypos;
        }
        if(ypos >= 600){
            lastY = 0;
            ypos = ypos - 600;
        }
        glfwSetCursorPos(window, xpos, ypos);
    }
    isEntered = entered;
}

void cursorCallback(GLFWwindow *window, double InxPos, double InyPos){
    if(isEntered or process) {
            bool clicked = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

            const auto xPos = static_cast<float>(InxPos);
            const auto yPos = static_cast<float>(InyPos);

            if(clicked) {
                if(process) {
                    float xOffset =  xPos - lastX;
                    float yOffset = lastY - yPos;
                    camera.update(xOffset, -yOffset);
                }
                else{
                    if(isEntered) {
                        process = true;
                    }
                }
            }
            else {
                process = false;
            }
            lastX = xPos;
            lastY = yPos;
        }
}

void scrollcb(GLFWwindow* window, double xoffset, double yoffset){
    camera.dis -= yoffset * 0.2f;
    if(camera.dis < 0.1){
        camera.dis = 0.1;
    }
}

void loadMap(const char* mapName){
    float* vert;

    if(vao){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    int width = 0;
    int height = 0;
    float step = 0.1f;
    generateMap(mapName, &vert, &width, &height, step);
    x = width * step / 2;
    y = height * step / 2;
    numVert = width * height * 6;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square) * width * height, vert, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);
    if(vert){
        isLoaded = true;
        free(vert);
    }
    
}