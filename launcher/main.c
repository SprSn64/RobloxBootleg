#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

SDL_Point windowScale = {640, 360};

SDL_FPoint mousePos;
SDL_MouseButtonFlags mouseFlags;

SDL_FRect buttonRect = {420, 240, 160, 32};
bool buttonHover, buttonPressed = false;

Uint64 last = 0;
Uint64 now = 0;
double deltaTime = 0;

float timer = 0;

SDL_Texture *fontTex = NULL;

void drawText(SDL_Renderer *renderer, SDL_Texture *texture, char* text, char charOff, short posX, short posY, short width, short height, short kern);

void HandleKeyInput();
SDL_Texture *newTexture(char* path);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
	SDL_SetAppMetadata("Example Renderer Clear", "1.0", NULL);
	
	if (!SDL_Init(SDL_INIT_VIDEO)){
		SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("shitty ass launcher", windowScale.x, windowScale.y, 0, &window, &renderer)){
		SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	fontTex = newTexture("assets/font.png");
	
	SDL_SetRenderVSync(renderer, 1);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	if (event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
	}
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *appstate){
	last = now;
	now = SDL_GetTicks();
	deltaTime = fmin(((double)now - (double)last) / 1000.0f, 1);
	timer += deltaTime;
	
	SDL_GetWindowSize(window, &windowScale.x, &windowScale.y);
	mouseFlags = SDL_GetMouseState(&mousePos.x, &mousePos.y);
	
	SDL_SetRenderDrawColor(renderer, 20, 22, 24, 255);  /* black, full alpha */
	SDL_RenderClear(renderer);  /* start with a blank canvas. */
	
	drawText(renderer, fontTex, "Commence thyne launchere!", 32, 0, 0, 8, 8, 7);
	
	buttonHover = (mousePos.x >= buttonRect.x && mousePos.y >= buttonRect.y && mousePos.x <= buttonRect.x + buttonRect.w && mousePos.y <= buttonRect.y  + buttonRect.h);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &(SDL_FRect){buttonRect.x - 1, buttonRect.y - 1, buttonRect.w + 2, buttonRect.h + 2});
	SDL_SetRenderDrawColor(renderer, 28 + 32 * buttonHover, 30 + 32 * buttonHover, 32 + 32 * buttonHover, 255);
	SDL_RenderFillRect(renderer, &buttonRect);	
	drawText(renderer, fontTex, "sure why not", 32, buttonRect.x + 4, buttonRect.y + 4, 8, 8, 7);	
	
	SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER * buttonHover));
	
	if(buttonHover && (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS && (mouseFlags & SDL_BUTTON_LMASK))){
		if(buttonPressed = false)buttonPressed = true;
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &buttonRect);
		return SDL_APP_SUCCESS;
	}else{
		buttonPressed = false;
	}
	
	SDL_RenderPresent(renderer);  /* put it all on the screen! */
	return SDL_APP_CONTINUE;  /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
	SDL_DestroyTexture(fontTex);
}

SDL_Texture *newTexture(char* path){
	SDL_Surface *texSurface = NULL;
	
	texSurface = IMG_Load(path);
	
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, texSurface);
	SDL_DestroySurface(texSurface);
	return texture;
}

void drawText(SDL_Renderer *renderer, SDL_Texture *texture, char* text, char charOff, short posX, short posY, short width, short height, short kern){
	for(int i=0; i<=strlen(text); i++){
		char charVal = (unsigned)text[i] - charOff;
		int xOff = (charVal % 16) * width;
		int yOff = floor((float)charVal / 16) * height;
		SDL_FRect sprRect = {xOff, yOff, width, height};
		SDL_FRect sprPos = {posX + kern * i, posY, width, height};
		SDL_RenderTexture(renderer, texture, &sprRect, &sprPos);
	}
}