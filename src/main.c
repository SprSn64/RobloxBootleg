#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
	#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <math.h>

// #include "cglm/cglm.h"

#include "math.h"
#include <types.h>
#include "loader.h"
#include "renderer.h"

const char *vertexShaderSource;
const char *fragmentShaderSource;

Vector2 windowScale = {640, 480};
Camera camera = {(Vector3){0, 0, 0.5}, (Vector3){0, 0, 0}};
float cameraMoveSpeed = 960.f;
float cameraRotSpeed = 12.f;

GLFWwindow *window = NULL;

#define KEYCOUNT 10
KeyMap keyList[KEYCOUNT];

double last = 0;
double now = 0;
double deltaTime = 0;
float timer = 0;

float triVerts[] = {
	-0.5f, -0.5f, -0.5f, 	1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,	0.f, 1.f, 0.f,
	0.5f, -0.5f, 0.5f,	0.f, 0.f, 1.f,
	-0.5f, -0.5f, 0.5f,	1.f, 1.f, 0.f,
	0.f, 0.5f, 0.f,		1.f, 0.f, 1.f,
};

unsigned int triFaces[] = {
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4,
	2, 1, 0,
	0, 3, 2,
};
	
void SDL_AppQuit(void *appstate, SDL_AppResult result){
	glfwDestroyWindow(window);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
}

void HandleKeyInput(){
	const bool* keyState = SDL_GetKeyboardState(NULL);
	for(int i = 0; i < 4; i++){
		keyList[i].down = keyState[keyList[i].scanCode];
	}
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
	printf("kill people\n");
	if (!glfwInit()){
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

	window = glfwCreateWindow(windowScale.x, windowScale.y, "Kill People", NULL, NULL);
	if (!window)
		return 1;

	glfwSetWindowSizeLimits(window, 320, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	const GLint shaderTimer = glGetUniformLocation(shaderProgram, "timer");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVerts), triVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triFaces), triFaces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float *localMatrix = initMatrix();
	float *viewMatrix = initMatrix();
	float *projMatrix = initMatrix(); // perspMatrix(45.f * DEG2RAD, windowScale.x / windowScale.y, 0.1f, 100.f);

	float *cameraMatrix = initMatrix();
	int cameraLoc = glGetUniformLocation(shaderProgram, "camera");

	int modelLoc = glGetUniformLocation(shaderProgram, "model");
	int viewLoc = glGetUniformLocation(shaderProgram, "view");
	int projLoc = glGetUniformLocation(shaderProgram, "proj");

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
}

SDL_AppResult SDL_AppIterate(void *appstate){
	last = now;
	now = glfwGetTime();
	deltaTime = min(((double)now - (double)last) / 1000.0f, 1);
	timer += deltaTime;
	
	windowScale = (Vector2){width, height};
	
	// badtimer++;
	glUseProgram(shaderProgram);
	// viewMatrix[14] = (float)sin(timer / 48);
	viewMatrix = initMatrix();
	cameraMatrix = initMatrix();
	camera.pos.x += (cos(camera.rot.y * RAD2DEG) * (keyList[2].down - keyList[3].down) + sin(camera.rot.y * RAD2DEG) * (keyList[1].down - keyList[0].down)) * cameraMoveSpeed * deltaTime;
	camera.pos.y += (keyList[5].down - keyList[4].down) * cameraMoveSpeed * deltaTime;
	camera.pos.z += (-sin(camera.rot.y * RAD2DEG) * (keyList[2].down - keyList[3].down) + cos(camera.rot.y * RAD2DEG) * (keyList[1].down - keyList[0].down)) * cameraMoveSpeed * deltaTime;
	camera.rot.y += (keyList[9].down - keyList[8].down) * cameraRotSpeed * deltaTime;
	camera.rot.x += (keyList[7].down - keyList[6].down) * cameraRotSpeed * deltaTime;
	matrixTranslate(cameraMatrix, camera.pos);
	matrixRotate(viewMatrix, (Vector3){camera.rot.x * RAD2DEG, camera.rot.y * RAD2DEG, camera.rot.z * RAD2DEG});
	// matrixRotate(localMatrix, (Vector3){0.f, deltaTime * 960, 0.f});
	// matrixRotate(viewMatrix, (Vector3){RAD2DEG * 23 * deltaTime, 0, 0});
	// matrixRotate(viewMatrix, (Vector3){0, RAD2DEG * 30 * deltaTime, 0});
	// matrixRotate(viewMatrix, (Vector3){0, 0, RAD2DEG * 9 * deltaTime});
	// viewMatrix[12] = (float)sin(RAD2DEG * timer * 24) / 2;
	// viewMatrix[13] = (float)cos(RAD2DEG * timer * 24) / 2;
	// viewMatrix[14] = (float)sin(RAD2DEG * timer * 12) / 2 + 0.5f;
	projMatrix = perspMatrix(80.f * DEG2RAD, (float)windowScale.x / windowScale.y, 0.1f, 100.f);
	// float shaderTimer = sin(timer) / 2 + 1;
	// float shaderTimer = 1;
	glViewport(0, 0, windowScale.x, windowScale.y);
	glClearColor(0.078f, 0.086f, 0.124f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, localMatrix);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projMatrix);
	glUniformMatrix4fv(cameraLoc, 1, GL_FALSE, cameraMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, sizeof(triFaces) / sizeof(int), GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
	
	//SDL_RenderPresent(renderer);  /* put it all on the screen! */
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

int main(int argc, char *argv[]){
	while (!glfwWindowShouldClose(window)){
	}
	glfwDestroyWindow(window);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}