/* ----------
-lsfml-main
-lsfml-graphics
-lsfml-system
-lsfml-window
*/
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <sstream>
#include <windows.h>

using namespace sf;
using namespace std;

int main() {
    HWND hwnd = GetConsoleWindow();//Make the toolbar transparent
    if (hwnd != NULL){
        SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hwnd, 0, 0, LWA_ALPHA);
    }
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    // Window dimensions
    const float WINDOW_WIDTH = 800;
    const float WINDOW_HEIGHT = 600;

    // Clock radius
    const float CLOCK_RADIUS = 250;
    const float CENTER_RADIUS = 10;

    // Hand lengths
    const float HOUR_HAND_LENGTH = CLOCK_RADIUS * 0.5;
    const float MINUTE_HAND_LENGTH = CLOCK_RADIUS * 0.7;
    const float SECOND_HAND_LENGTH = CLOCK_RADIUS * 0.9;

    // Hand colors
    const Color HOUR_COLOR = Color::White;
    const Color MINUTE_COLOR = Color::White;
    const Color SECOND_COLOR = Color::Red;

    // Create the window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Analog Clock", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60); // Cap the frame rate

    // -----------------------
    // Clock face
    // -----------------------
    CircleShape clockFace(CLOCK_RADIUS);
    // Create a gradient for the clock face
    VertexArray gradient(TriangleFan, 6);
    gradient[0].position = Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    gradient[0].color = Color(50, 50, 50); // Darker center

    for (int i = 1; i <= 5; ++i) {
        float angle = i * 72.0f * (3.14159f / 180.0f); // 5 points around the circle
        float x = WINDOW_WIDTH / 2 + CLOCK_RADIUS * cos(angle);
        float y = WINDOW_HEIGHT / 2 + CLOCK_RADIUS * sin(angle);
        gradient[i].position = Vector2f(x, y);
        gradient[i].color = Color(100, 100, 100); // Lighter edge
    }
    gradient[5].position = gradient[1].position;
    gradient[5].color = gradient[1].color;

    clockFace.setFillColor(Color::Transparent); // Make the circle transparent, so we see the gradient
    clockFace.setOutlineColor(Color::White);
    clockFace.setOutlineThickness(2);
    clockFace.setPosition(WINDOW_WIDTH / 2 - CLOCK_RADIUS, WINDOW_HEIGHT / 2 - CLOCK_RADIUS);

     // Add a shadow to the clock face.  This is done by drawing a filled circle underneath, offset a bit.
    CircleShape shadow(CLOCK_RADIUS);
    shadow.setFillColor(Color(0, 0, 0, 100)); // Semi-transparent black
    shadow.setPosition(WINDOW_WIDTH / 2 - CLOCK_RADIUS + 5, WINDOW_HEIGHT / 2 - CLOCK_RADIUS + 5); // Offset the shadow

    // Center point
    CircleShape centerPoint(CENTER_RADIUS);
    centerPoint.setFillColor(Color::White);
    centerPoint.setPosition(WINDOW_WIDTH / 2 - CENTER_RADIUS, WINDOW_HEIGHT / 2 - CENTER_RADIUS);

    // Hour hand
    RectangleShape hourHand(Vector2f(7, HOUR_HAND_LENGTH)); // Thicker hand
    hourHand.setFillColor(HOUR_COLOR);
    hourHand.setOrigin(3.5, HOUR_HAND_LENGTH);
    hourHand.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Minute hand
    RectangleShape minuteHand(Vector2f(5, MINUTE_HAND_LENGTH)); // Thicker hand
    minuteHand.setFillColor(MINUTE_COLOR);
    minuteHand.setOrigin(2.5, MINUTE_HAND_LENGTH);
    minuteHand.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Second hand
    RectangleShape secondHand(Vector2f(3, SECOND_HAND_LENGTH)); // Slightly thicker
    secondHand.setFillColor(SECOND_COLOR);
    secondHand.setOrigin(1.5, SECOND_HAND_LENGTH);
    secondHand.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    // Tick marks
//    vector<RectangleShape> tickMarks;
//    for (int i = 0; i < 60; ++i) {
//        float length = (i % 5 == 0) ? 15 : 8; // Longer ticks for hours, shorter for minutes
//        float thickness = (i % 5 == 0) ? 3 : 1;    // Thicker hour ticks
//        // RectangleShape tick(Vector2f(thickness, length));
//        tick.setFillColor(Color::White);
//        tick.setOrigin(thickness / 2, length);
//        tick.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
//        tick.setRotation(i * 6);
//        tickMarks.push_back(tick);
//    }

    // Numbers
    Font font;
    if (!font.loadFromFile("Roboto-Regular.ttf")) { // Use Roboto font
        cerr << "Failed to load font!" << endl;
        return 1;
    }
    vector<Text> numbers;
    for (int i = 1; i <= 12; ++i) {
        Text number;
        number.setFont(font);
        number.setString(to_string(i));
        number.setCharacterSize(24); // Larger font size
        number.setFillColor(Color::White);
        // Position the numbers.
        float angle = (i - 3) * 30.0f * (3.14159f / 180.0f);
        float x = WINDOW_WIDTH / 2 + (CLOCK_RADIUS * 0.8f) * cos(angle);
        float y = WINDOW_HEIGHT / 2 + (CLOCK_RADIUS * 0.8f) * sin(angle);
        number.setPosition(x, y);
        // Center the number
        FloatRect bounds = number.getLocalBounds();
        number.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        numbers.push_back(number);
    }

    // Load the background image
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) { // Replace with your image file
        cerr << "Failed to load background image!" << endl;
        return 1;
    }
    RectangleShape background(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    background.setTexture(&backgroundTexture);


    // Main loop
    while (window.isOpen()) {
        // Event handling
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Get current time
        time_t now = time(0);
        tm* ltm = localtime(&now);
        int hour = ltm->tm_hour % 12;
        int minute = ltm->tm_min;
        int second = ltm->tm_sec;

        // Calculate hand angles
        float secondAngle = second * 6.0f;
        float minuteAngle = (minute + second / 60.0f) * 6.0f;
        float hourAngle = (hour + minute / 60.0f) * 30.0f;

        // Rotate hands
        secondHand.setRotation(secondAngle);
        minuteHand.setRotation(minuteAngle);
        hourHand.setRotation(hourAngle);

        // Clear the window
        window.clear(Color(30, 30, 30));

        // Draw everything
        window.draw(background); // Draw background first
        window.draw(shadow); // Draw shadow first
        window.draw(clockFace);
        // window.draw(gradient); // Draw the gradient
//        for (const auto& tick : tickMarks) {
//            window.draw(tick);
//        }
        for (const auto& number : numbers) {
            window.draw(number);
        }
        window.draw(centerPoint);
        window.draw(hourHand);
        window.draw(minuteHand);
        window.draw(secondHand);

        // Display the window
        window.display();
    }

    return 0;
}
