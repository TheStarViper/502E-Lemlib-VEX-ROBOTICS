#include "graphics_renderer.hpp"


/*

things to add

1.buttons with push down animation
2.sliders
3.bar graphs
4a.pie charts
4b.donut charts
5.progress bars
6.shapes with a custom amount of vertices and coordinates
7.transitions between screens
8a.animations with easings from easings.net
8b.custom easings
9.ellipses
10.arcs
11.bezier curves
12.custom font rendering (bitmap font)
13.rotation and scaling animations
14.renderable gradients including horizontal,verticle,diagonal,radial,diamond,conic
15.dials
16.checkerboard
17.color palettes
18.debug terminal page with a print_to_terminal function
19.fade in animations
*/


namespace Graphics {

// Convert a hex color code string (e.g., "#FF5733") into a uint32_t color
uint32_t hexColorFromString(const std::string& hex) {
    if (hex.length() != 7 || hex[0] != '#') {
        throw std::invalid_argument("Invalid hex color code format.");
    }

    uint32_t color = 0;
    for (size_t i = 1; i < hex.length(); ++i) {
        if (!std::isxdigit(hex[i])) {
            throw std::invalid_argument("Hex code contains non-hex characters.");
        }
        color <<= 4;
        if (hex[i] >= '0' && hex[i] <= '9') {
            color |= (hex[i] - '0');
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            color |= (hex[i] - 'A' + 10);
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            color |= (hex[i] - 'a' + 10);
        }
    }
    return color;
}

// Function to set the drawing color using an HTML hex code
void setHexColor(const std::string& hexCode) {
    uint32_t color = hexColorFromString(hexCode);  // Convert the hex code to uint32_t
    pros::screen::set_pen(color);  // Set the drawing color
}

// Function to clear the screen
void clearScreen() {
    pros::screen::erase();  // Clears the screen
}

// Function to erase a rectangle using the erase_rect function
void eraseRectangle(int x, int y, int width, int height) {
    pros::screen::erase_rect(x, y, width, height);  // Erase the rectangle area
}

// Function to erase a circle using the erase_circle function
void eraseCircle(int x, int y, int radius) {
    pros::screen::erase_circle(x, y, radius);  // Erase the circle
}

// Function to draw a filled rectangle
void drawRectangle(int x, int y, int width, int height, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    pros::screen::fill_rect(x, y, width, height);  // Draw the filled rectangle
}

// Function to draw a filled circle
void drawCircle(int x, int y, int radius, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    pros::screen::fill_circle(x, y, radius);  // Draw the filled circle
}

// Function to draw a rectangle outline
void drawRectangleOutline(int x, int y, int width, int height, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    pros::screen::draw_rect(x, y, width, height);  // Draw the rectangle outline
}

// Function to draw a circle outline
void drawCircleOutline(int x, int y, int radius, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    pros::screen::draw_circle(x, y, radius);  // Draw the circle outline
}

// Function to draw text on the screen
void drawText(int x, int y, std::string text, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    pros::screen::print(pros::E_TEXT_MEDIUM, x, y,text.c_str());
}

// Function to draw a rounded rectangle
void drawRoundedRectangle(int x, int y, int width, int height, int radius, const std::string& hexColor) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code

    // Draw the rectangle body
    pros::screen::fill_rect(x + radius, y, width - 2 * radius, height); // Middle section (top and bottom)
    pros::screen::fill_rect(x, y + radius, width, height - 2 * radius); // Middle section (left and right)

    // Draw the four corners
    pros::screen::fill_circle(x + radius, y + radius, radius); // Top-left corner
    pros::screen::fill_circle(x + width - radius, y + radius, radius); // Top-right corner
    pros::screen::fill_circle(x + radius, y + height - radius, radius); // Bottom-left corner
    pros::screen::fill_circle(x + width - radius, y + height - radius, radius); // Bottom-right corner
}

// Function to rotate a point (x, y) around a center (cx, cy) by an angle in radians
void rotatePoint(int &x, int &y, int cx, int cy, double angle) {
    int tempX = x;
    int tempY = y;

    x = cx + (tempX - cx) * cos(angle) - (tempY - cy) * sin(angle);
    y = cy + (tempX - cx) * sin(angle) + (tempY - cy) * cos(angle);
}

// Function to draw a rotated rectangle
void drawRotatedRectangle(int x, int y, int width, int height, int radius, const std::string& hexColor, double angle) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code

    // Rotate the four corners of the rectangle
    int x1 = x, y1 = y;
    int x2 = x + width, y2 = y;
    int x3 = x, y3 = y + height;
    int x4 = x + width, y4 = y + height;

    rotatePoint(x1, y1, x + width / 2, y + height / 2, angle);
    rotatePoint(x2, y2, x + width / 2, y + height / 2, angle);
    rotatePoint(x3, y3, x + width / 2, y + height / 2, angle);
    rotatePoint(x4, y4, x + width / 2, y + height / 2, angle);

    // Draw the rotated rectangle by connecting the rotated points
    pros::screen::draw_line(x1, y1, x2, y2);
    pros::screen::draw_line(x2, y2, x4, y4);
    pros::screen::draw_line(x4, y4, x3, y3);
    pros::screen::draw_line(x3, y3, x1, y1);
}

// Function to draw rotated text
void drawRotatedText(int x, int y, std::string text, const std::string& hexColor, double angle) {
    setHexColor(hexColor);  // Set the drawing color using the HTML hex code
    // For simplicity, we're not rotating individual characters, just the text center
    int textLength = text.length() * 6;  // Assuming average character width
    int textX = x - textLength / 2;
    int textY = y;

    // Rotation logic for text could go here, but PROS doesn't support rotation of text by default
    pros::screen::print(pros::E_TEXT_MEDIUM, textY, textX,text.c_str());
}
/*
void renderLoop() {
    // Example of usage of some drawing functions
    clearScreen();  // Clear the screen before rendering new shapes

    // Draw a filled rectangle
    //drawRectangle(50, 50, 100, 50, "#FF5733");

    // Draw a circle outline
    drawCircleOutline(200, 200, 50, "#33FF57");

    // Draw a rotated rectangle
    drawRotatedRectangle(300, 100, 120, 60, 10, "#5733FF", M_PI / 4);  // 45 degree rotation (in radians)

    // Draw rotated text (while rotation is simulated)
    drawRotatedText(400, 250, "Hello, World!", "#FF33FF", M_PI / 4);  // 45 degree rotation

    // Draw a rounded rectangle
    drawRoundedRectangle(500, 50, 150, 75, 20, "#FF5733");

    // Wait for a short amount of time before the next frame (for example, 20ms)
    pros::Task::delay(20);
}
//pros::Task Renderer(renderLoop,TASK_PRIORITY_DEFAULT+1);*/
}
