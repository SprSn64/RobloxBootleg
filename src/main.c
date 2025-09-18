#include <glad/gl.h>
#include <GLFW/glfw3.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "math.h"
#include <types.h>
#include "loader.h"
#include "renderer.h"

#include "game/object.h"
#include "game/model.h"
#include "game/behaviors.h"

const char *vertexShaderSource;
const char *fragmentShaderSource;

Vector2 windowScale = {640, 480};
Camera camera = {(Vector3){0, 0, 0.5}, (Vector3){0, 0, 0}};
F32 cameraMoveSpeed = 960.f;
F32 cameraRotSpeed = 12.f;

GLFWwindow *window = NULL;

#define KEYCOUNT 10
KeyMap keyList[KEYCOUNT];

F64 last = 0;
F64 now = 0;
F64 deltaTime = 0;
F32 timer = 0;

void error_callback(S32 error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void window_close_callback(GLFWwindow *window)
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
    windowScale = (Vector2){width, height};
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    for (int i = 0; i < KEYCOUNT; i++)
    {
        keyList[i].down = glfwGetKey(window, keyList[i].scanCode);
    }
}

int main(int argc, char *argv[])
{
    printf("kill people\n");

    if (!glfwInit())
    {
#ifdef _WIN32
        MessageBox(NULL, "OpenGL... no workies!", "Whoops!", 16 | MB_OK);
#else
        fprintf(stderr, "OpenGL... no workies!, Whoops!");
#endif
        return 1;
    }

    vertexShaderSource = loadTextFile("assets/shaders/default.vert");
    fragmentShaderSource = loadTextFile("assets/shaders/default.frag");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwSetErrorCallback(error_callback);
    window = glfwCreateWindow(windowScale.x, windowScale.y, "Kill People", NULL, NULL);
    if (!window)
        return 1;

    glfwSetWindowSizeLimits(window, 320, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // compile shaders
    U32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    U32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    U32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    createObject(bhv_Pyramid, MODEL_PYRAMID, (Vector3){1, 0, 0}, (Vector3){0, 0, 0});
    createObject(bhv_Static_Cube, MODEL_CUBE, (Vector3){0, 0, 0}, (Vector3){0, 0, 0});

    F32 *localMatrix = initMatrix();
    F32 *viewMatrix = initMatrix();
    F32 *projMatrix = initMatrix();
    F32 *cameraMatrix = initMatrix();

    S32 cameraLoc = glGetUniformLocation(shaderProgram, "camera");
    S32 modelLoc = glGetUniformLocation(shaderProgram, "model");
    S32 viewLoc = glGetUniformLocation(shaderProgram, "view");
    S32 projLoc = glGetUniformLocation(shaderProgram, "proj");

    matrixTranslate(viewMatrix, (Vector3){0, 0, -0.5f});
    glClearColor(0.078f, 0.086f, 0.124f, 1.0f);

    keyList[0].scanCode = GLFW_KEY_W;
    keyList[1].scanCode = GLFW_KEY_S;
    keyList[2].scanCode = GLFW_KEY_A;
    keyList[3].scanCode = GLFW_KEY_D;
    keyList[4].scanCode = GLFW_KEY_SPACE;
    keyList[5].scanCode = GLFW_KEY_LEFT_SHIFT;
    keyList[6].scanCode = GLFW_KEY_UP;
    keyList[7].scanCode = GLFW_KEY_DOWN;
    keyList[8].scanCode = GLFW_KEY_LEFT;
    keyList[9].scanCode = GLFW_KEY_RIGHT;

    last = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        now = glfwGetTime();
        deltaTime = min((double)(now - last) / 1000.0, 1);
        last = now;
        timer += deltaTime;

        glUseProgram(shaderProgram);
        viewMatrix = initMatrix();
        cameraMatrix = initMatrix();

        camera.pos.x += (cos(camera.rot.y * RAD2DEG) * (keyList[2].down - keyList[3].down) + sin(camera.rot.y * RAD2DEG) * (keyList[1].down - keyList[0].down)) * cameraMoveSpeed * deltaTime;
        camera.pos.y += (keyList[5].down - keyList[4].down) * cameraMoveSpeed * deltaTime;
        camera.pos.z += (-sin(camera.rot.y * RAD2DEG) * (keyList[2].down - keyList[3].down) + cos(camera.rot.y * RAD2DEG) * (keyList[1].down - keyList[0].down)) * cameraMoveSpeed * deltaTime;

        camera.rot.y += (keyList[9].down - keyList[8].down) * cameraRotSpeed * deltaTime;
        camera.rot.x += (keyList[7].down - keyList[6].down) * cameraRotSpeed * deltaTime;

        matrixTranslate(cameraMatrix, camera.pos);
        matrixRotate(viewMatrix, (Vector3){camera.rot.x * RAD2DEG, camera.rot.y * RAD2DEG, camera.rot.z * RAD2DEG});

        projMatrix = perspMatrix(80.f * DEG2RAD, (float)windowScale.x / windowScale.y, 0.1f, 100.f);

        glViewport(0, 0, windowScale.x, windowScale.y);
        glClearColor(0.078f, 0.086f, 0.124f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix);
        glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, cameraMatrix);

        for (int i = 0; i < objectCount; i++)
        {
            F32 *modelMatrix = initMatrix();
            currentProcObj = objects[i];

            objects[i].timer += deltaTime;

            if (objects[i].bhv.update)
            {
                objects[i].bhv.update(&objects[i]);
            }

            matrixTranslate(modelMatrix, objects[i].pos);
            matrixRotate(modelMatrix, objects[i].rot);
            // matrixScale(modelMatrix, objects[i].scale);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);

            glBindVertexArray(objects[i].VAO);
            glDrawElements(GL_TRIANGLES, objects[i].indexCount, GL_UNSIGNED_INT, 0);

            free(modelMatrix);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    for (int i = 0; i < objectCount; i++)
    {
        glDeleteVertexArrays(1, &objects[i].VAO);
        glDeleteBuffers(1, &objects[i].VBO);
        glDeleteBuffers(1, &objects[i].EBO);
    }
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
