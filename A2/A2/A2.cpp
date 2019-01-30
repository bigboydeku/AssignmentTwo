// A2.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
This program belongs to Denis Nadarevic (104445626). Any attempt to duplicate/utilize for commercial without permission
could face legal action being taken against. This program is for educational purposes.
*/
//

#include "pch.h"
#include <stdio.h>
#include <cstdio>
#include <algorithm>
#include <iostream>
#include <SDL.h>
#include <math.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

void menu();
void refresh(SDL_Surface * canvas, SDL_Renderer * renderer, SDL_Texture * texture);
void drawPoints(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas);
void drawLine(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas);
void drawLineAlg(uint32_t pixels[][SCREEN_WIDTH], int x1, int y1, int x2, int y2, int colour);
void drawCircleAlg(uint32_t pixels[][SCREEN_WIDTH], int x, int y, int height, int width, int colour);
void drawCircle(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas);

int main(int argc, char* args[]) {
	int userInput = 9;

	//Verify if SDL is initialized
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not initialize sdl2: %s\n", SDL_GetError());
		return 1;
	}
	//initialize window and verify if it works
	SDL_Window* window = SDL_CreateWindow("hello_sdl2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//create the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0); //the "0" lets the SDL2 choose what's best.
	if (renderer == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not create renderer: %s\n", SDL_GetError());
		return 1;
	}
	
	//create canvas
	SDL_Surface* canvas = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_PIXELFORMAT_RGBA8888);
	if (canvas == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not create surface: %s\n", SDL_GetError());
		return 1;
	}


	//create texture
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "could not create texture: %s\n", SDL_GetError());
		return 1;
	}

	//fill canvas with white
	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF));
	uint32_t(*pixels)[SCREEN_WIDTH] = (uint32_t(*)[SCREEN_WIDTH]) canvas->pixels;
	do
	{
		menu();
		scanf_s("%d", &userInput);
		switch (userInput)
		{
		case 1: //1. Draw points
			drawPoints(pixels, canvas);
			refresh(canvas, renderer, texture);
			break;
		case 2: //2. draw lines
			drawLine(pixels, canvas);
			refresh(canvas, renderer, texture);
			break;
		case 3: //draw circles
			drawCircle(pixels, canvas);
			refresh(canvas, renderer, texture);
			break;
		case 4: //end program, released resources
			SDL_DestroyTexture(texture);
			SDL_FreeSurface(canvas);
			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			printf("Bye.\n");
			SDL_Quit();
			break;
		default:
			printf("Invalid choice. Try again.\n");
			break;
		}


	} while (userInput != 4);
	return 0;
}


/*
Diplays menu
*/
void menu()
{
	printf("Welcome to assignment 1~\n");
	printf("1. Draw points\n2. Draw a line\n3. Draw a circle\n4. Exit\n>>");
}

/*
Refreshes and updates textures
*/
void refresh(SDL_Surface * canvas, SDL_Renderer * renderer, SDL_Texture * texture)
{
	SDL_UpdateTexture(texture, NULL, canvas->pixels, canvas->pitch);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

/*
Draws points from 1-5 depending on input
Input: pixels array
*/
void drawPoints(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas)
{
	int numPix = 0;
	while (numPix < 1 || numPix > 5) {
		printf("How many points? >> ");
		scanf_s("%d", &numPix);
	}

	int points[5][3]; //stores information of each point in a temporary array
	for (int i = 0; i < numPix; i++) {
		printf("Point %d, type the following info (x y colour) : >> ", i + 1);
		scanf_s("%d %d %x", &points[i][0], &points[i][1], &points[i][2]);
	}
	//for (int a = 0; a < SCREEN_HEIGHT; a++)
	//{
	//	for (int b = 0; b < SCREEN_WIDTH; b++)
	//	{
	//		pixels[a][b] = white;
	//
	//	}
	//}
	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF));


	for (int j = 0; j <= numPix; j++)
	{
		int *p = &points[j][0];
		if ((p[1] >= 0 && p[1] <= SCREEN_HEIGHT) && (p[0] >= 0 && p[0] <= SCREEN_WIDTH)) { //verify if pixel is in the screen
			pixels[p[1]][p[0]] = p[2]; //if its in the screen, set the colour of the pixel
		}
	}
}

/*
Draws a normal line from one point to another, translates it and rotates it
Input: pixels array
*/
void drawLine(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas)
{
	int one[2], two[2], colour; //coordinates and colour
	//one, two arrays mean point one and point two

	//collects users line info
	printf("line information (x1, y1, x2, y2, color): >> ");
	scanf_s("%d %d %d %d %x", &one[0], &one[1], &two[0], &two[1], &colour);

	//collects their translation request
	int tx, ty;
	printf("translate how far? (x translation, y translation): >> ");
	scanf_s("%d %d", &tx, &ty);

	//collects info for angled line
	int degrees;
	printf("degree of rotation: >> ");
	scanf_s("%d", &degrees);
	float ang = (float)(degrees * (M_PI / 180));
	//the following equation translates any line to the origin
	//this is for simpler rotation
	int mid[2] = { (one[0] + two[0]) / 2, (one[1] + two[1]) / 2 }; //midpoint
	
	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF));
	
	drawLineAlg(pixels, one[0], one[1], two[0], two[1], colour); //normal line
	drawLineAlg(pixels, one[0] + tx, one[1] + ty, two[0] + tx, two[1] + ty, colour); //translated line
	int origin_one[2] = { one[0] - mid[0],	one[1] - mid[1] }; //moves point one relative to origin
	int origin_two[2] = { two[0] - mid[0],	two[1] - mid[1] }; //moves point two relative to origin
	
	//rotated first point
	origin_one[0] = (int)(cos(ang) * origin_one[0] - sin(ang) * origin_one[1]);
	origin_one[1] = (int)(sin(ang) * origin_one[0] + cos(ang) * origin_one[1]);
	//rotate second point
	origin_two[0] = (int)(cos(ang) * origin_two[0] - sin(ang) * origin_two[1]);
	origin_two[1] = (int)(sin(ang) * origin_two[0] + cos(ang) * origin_two[1]);
	drawLineAlg(pixels, one[0] + mid[0], one[1] + mid[1], two[0] + mid[0], two[1] + mid[1], colour); //rotated line	
}

/*
Draws a circle, translates it, and scales it
Input: pixels array
*/
void drawCircle(uint32_t pixels[][SCREEN_WIDTH], SDL_Surface * canvas)
{
	int x, y;
	int colour;
	int radius;
	int xt, yt; //translation
	int xs, ys; //scale

	printf("Location of circle (x, y, colour, radius): >> ");
	scanf_s("%d %d %x %d", &x, &y, &colour, &radius);

	printf("Location to translate (xt, yt): >> ");
	scanf_s("%d %d", &xt, &yt);

	printf("Increase size to (xs, ys): >> ");
	scanf_s("%d %d", &xs, &ys);

	SDL_FillRect(canvas, NULL, SDL_MapRGB(canvas->format, 0xFF, 0xFF, 0xFF));

	//inputs have two radius since circles have the same length and width
	drawCircleAlg(pixels, x, y, radius, radius, colour); //normal circle
	drawCircleAlg(pixels, x + xt, y + yt, radius, radius, colour); //translated circle
	drawCircleAlg(pixels, x + xt, y + yt, radius + xs, radius + ys, colour); //scaled circle + translated

}

//two different size inputs to generalize the algorithm so it accepts ellipses as well
/*
this is an aiding function that determines how to fill a circle of specified radius, position, and colour
*/
void drawCircleAlg(uint32_t pixels[][SCREEN_WIDTH], int x, int y, int height, int width, int colour)
{
	for (int j = -height; j <= height; j++) // from [-R,R]
	{
		for (int i = -width; i <= width; i++) // from [-R,R] again since its a circle
		{
			//this was trial and error to determine if the pixel is inside the circle
			//if inside, fill it.
			if (i * i * height * height + j * j * width * width <= height * height * width * width)
			{
				pixels[y + j][x + i] = colour;
			}
		}
	}
}

void drawLineAlg(uint32_t pixels[][SCREEN_WIDTH], int x1, int y1, int x2, int y2, int colour)
{
	//if decreasing from first point to second, reverse
	//this is for simplicity
	if (x2 < x1)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	//if slope is 0, no point in using more complex algoritm, just draw down!
	if (x2 == x1)
	{
		if (y2 < y1)
			std::swap(y1, y2);

		for (int y = y1; y < y2; y++)
			pixels[y][x1] = colour;
	}
	else
	{
		//simple algorithm to draw from first point to second point
		int m = (y2 - y1) / (x2 - x1); //slope formula
		int c = y2 - m * x2; //y-intercept formula

		for (int x = x1; x <= x2; x++)
		{
			int y = m * x + c;
			if ((y >= 0 && y < SCREEN_HEIGHT) && (x >= 0 && x < SCREEN_WIDTH)) { // Checking to see if the coords are on the screen
				pixels[y][x] = colour;
			}
		}
	}
}
