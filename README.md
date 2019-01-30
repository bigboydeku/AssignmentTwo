# AssignmentTwo
Graphic Fundamentals

# Before Starting the Assignment
I was a little overwhelmed because I've never really applied C to anything outside of the course; however, this was interesting to do. If I could ask for anything for future assignment, it would be to receive resources to go off from so I can steadily learn the concepts and such. Overall, this was a challenging but good learning experience.

# Functions
I coded the information-gathering functions and the algorithms seperately to organize my thoughts. Below I show my algorithms for drawing. The functions that I did not include in this README are just used to collect the information from the user and utilize the algorithms below to draw original, translated, rotated, and scaled shapes and lines.

The main function loops around until the user is finished with their choices. It displays a menu that the user can choose from.

# Draw Points Algorithm
After clearing the screen, this algorithm draws a new line. It checks if the pixels are actually on the screen and, if so, it sets the pixel's colour to the user's choice.
```
for (int j = 0; j <= numPix; j++)
	{
		int *p = &points[j][0];
		if ((p[1] >= 0 && p[1] <= SCREEN_HEIGHT) && (p[0] >= 0 && p[0] <= SCREEN_WIDTH)) { //verify if pixel is in the screen
			pixels[p[1]][p[0]] = p[2]; //if its in the screen, set the colour of the pixel
		}
	}
```

# Draw Circle Algorithm
This is an aiding function that determines how to fill a circle of specified radius, position, and colour.
```
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
```

# Draw Line Algorithm
This was coded with certain cases in mind, such as horizontal lines. 
```
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
```
