#include "main.h"

namespace Graphics {

// Convert a hex color code string (e.g., "#FF5733") into a uint32_t color
uint32_t hexColorFromString(const std::string& hex);

// Function to set the drawing color using an HTML hex code
void setHexColor(const std::string& hexCode);

// Function to clear the screen
void clearScreen();

// Function to erase a rectangle using the erase_rect function
void eraseRectangle(int x, int y, int width, int height);

// Function to erase a circle using the erase_circle function
void eraseCircle(int x, int y, int radius);

// Function to draw a filled rectangle
void drawRectangle(int x, int y, int width, int height, const std::string& hexColor);

// Function to draw a filled circle
void drawCircle(int x, int y, int radius, const std::string& hexColor);

// Function to draw a rectangle outline
void drawRectangleOutline(int x, int y, int width, int height, const std::string& hexColor);

// Function to draw a circle outline
void drawCircleOutline(int x, int y, int radius, const std::string& hexColor);

// Function to draw text on the screen
void drawText(int x, int y, std::string text, const std::string& hexColor);

// Function to draw a rounded rectangle
void drawRoundedRectangle(int x, int y, int width, int height, int radius, const std::string& hexColor);

// Function to rotate a point (x, y) around a center (cx, cy) by an angle in radians
void rotatePoint(int &x, int &y, int cx, int cy, double angle);

// Function to draw a rotated rectangle
void drawRotatedRectangle(int x, int y, int width, int height, int radius, const std::string& hexColor, double angle);

// Function to draw rotated text
void drawRotatedText(int x, int y, std::string text, const std::string& hexColor, double angle);

// Main rendering loop
//void renderLoop();

} // namespace Graphics