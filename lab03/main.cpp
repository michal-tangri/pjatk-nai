#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/base.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char **argv)
{

    using namespace cv;

    // Image source
    VideoCapture cap(0);

    // Default window size
    int window_size_x = 320;
    int window_size_y = 200;

    // Ranges in which the program will find an object by color
    int h = 0, s = 183, v = 111;
    int high_h = 35, high_s = 255, high_v = 233;

    // Structuring element for morphology
    static auto ellipse = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));

    // Matrices for source input
    Mat frame;
    Mat frame_hsv;

    // Check is source input has been opened properly
    if (!cap.isOpened()) {
        std::cerr << "Cannot open source input" << std::endl;
        return -1;
    }

    // Simple reading of arguments to resize image
    if (argc == 2) {
        window_size_x = atoi(argv[1]);
        window_size_y = atoi(argv[1]);
    } else if (argc == 3) {
        window_size_x = atoi(argv[1]);
        window_size_y = atoi(argv[2]);
    }

    
    do {
        // Read from camera input resize and flip image
        cap.read(frame);
        resize(frame, frame, {window_size_x, window_size_y});
        flip(frame, frame, 1);

        // Change colors
        cvtColor(frame, frame_hsv, COLOR_BGR2HSV);

        // Find color in range (yellow ducks)
        inRange(frame_hsv, Scalar(h, s, v), Scalar(high_h, high_s, high_v), frame_hsv);

        // Remove noise
        morphologyEx(frame_hsv, frame_hsv, MORPH_CLOSE, ellipse);
		morphologyEx(frame_hsv, frame_hsv, MORPH_OPEN, ellipse);

        // Get objects contours
        std::vector<std::vector<Point>> contours;
		findContours(frame_hsv, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        // Sort contours starting with one with the biggest area
        sort(contours.begin(), contours.end(),
				 [](auto &a, auto &b) {
					 return contourArea(a, false) > contourArea(b, false);
				 });

        // Draw contours to original image
        for (int i = 0; i < contours.size(); i++) {
			drawContours(frame, contours, i, Scalar(0, 255, 0));
            std::string text = std::to_string(i + 1) + ": " + std::to_string(contours.at(i).size());
            putText(frame, text, {30, (i + 1) * 30}, FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
		}

        // Find centroids for two biggest contours and draw a line between them
        if (contours.size() >= 2) {
            int x[2], y[2];
            for (int i = 0; i < 2; i++) {
                auto mnts = moments(contours.at(i));
                x[i] = int(mnts.m10 / mnts.m00);
                y[i] = int(mnts.m01 / mnts.m00);
            }

            // Check if objects are on the same level (with error margin)
            if((y[0] < y[1] + 50) && (y[0] > y[1] - 50))
                line(frame, {x[0], y[0]}, {x[1], y[1]}, {255, 0, 0, 255}, 3);
        }

        // Show original image with countours
        imshow("Original", frame);

        // Show found objects
        imshow("HSV", frame_hsv);

        // Rules what to do when a specific key is pressed
        char pressed_key = waitKey(1);
        if (pressed_key == 'x')
        {
            Rect2d selection = selectROI("HSV", frame_hsv);
            imwrite("saved.jpg", Mat(frame_hsv, selection));
        }
        else if (pressed_key == 27)
        {
            break;
        }

    } while (true);

    return 0;
}