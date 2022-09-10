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

void cursorcb(GLFWwindow* window, double posx, double posy);

Camera camera;

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Example", nullptr, nullptr);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursorcb);
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
    }char log[512];
    glGetShaderInfoLog(VertexShader, 512, 0, log);
    std::cout << log << std::endl;

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
    unsigned int view = glGetUniformLocation(m_Program, "view");
    unsigned int proj = glGetUniformLocation(m_Program, "proj");

    float* vert;
    constexpr int width = 2;
    constexpr int height = 4;
    constexpr float step = 0.5f;
    generateMap(width, height, &vert, step);
    constexpr float x = width * step / 2;
    constexpr float y = height * step / 2;
    constexpr int numVert = width * height * 6;

    glClearColor(0.31f, 0.31f, 0.31f, 1.0f);

    unsigned int vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Square) * width * height, vert, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
    glEnableVertexAttribArray(0);
    camera.dis = 2.0f;
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(m_Program);
        
        glUniformMatrix4fv(view, 1, GL_FALSE, (float*)glm::value_ptr(camera.getView()));
        glUniformMatrix4fv(proj, 1, GL_FALSE, (float*)glm::value_ptr(camera.getPerspective()));
        glUniform3f(pos, x, 0, y);


        //glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);
        glPolygonMode(GL_FRONT, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, numVert);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::ShowDemoWindow();

        if(ImGui::Begin(" Maap")){

            
            ImGui::BeginChild("Map");
            

            ImGui::EndChild();
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

    free(vert);

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

void cursorcb(GLFWwindow* window, double posx, double posy){
    float xOffset = posx - (400);
    float yOffset = posy - (300);

    camera.update(xOffset, yOffset);

    glfwSetCursorPos(window, (400), (300));
}
