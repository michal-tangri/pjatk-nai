#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/base.hpp>
#include <iostream>

int main(int argc, char **argv)
{

    cv::VideoCapture cap(0);

    int window_size_x = 320;
    int window_size_y = 200;
    int h = 0, s = 0, v = 0;
    int high_h = 255, high_s = 255, high_v = 255;

    if (!cap.isOpened())
    {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }

    std::cout << "Video size: " << cap.get(cv::CAP_PROP_FRAME_WIDTH) << "x" << cap.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;

    if (argc == 2)
    {
        window_size_x = atoi(argv[1]);
        window_size_y = atoi(argv[1]);
    }
    else if (argc == 3)
    {
        window_size_x = atoi(argv[1]);
        window_size_y = atoi(argv[2]);
    }

    cv::Mat frame, frame_blur, frame_hsv;
    cap.read(frame);
    cv::resize(frame, frame, {window_size_x, window_size_y});
    cv::GaussianBlur(frame, frame_blur, cv::Size(7, 7), 0);

    cv::namedWindow("Trackbars", cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("H", "Trackbars", &h, 255);
    cv::createTrackbar("S", "Trackbars", &s, 255);
    cv::createTrackbar("V", "Trackbars", &v, 255);
    cv::createTrackbar("High H", "Trackbars", &high_h, 255);
    cv::createTrackbar("High S", "Trackbars", &high_s, 255);
    cv::createTrackbar("High V", "Trackbars", &high_v, 255);

    do
    {
        cv::imshow("Blur", frame_blur);
        cv::cvtColor(frame_blur, frame_hsv, cv::COLOR_BGR2HSV);
        cv::inRange(frame_hsv, cv::Scalar(h, s, v), cv::Scalar(high_h, high_s, high_v), frame_hsv);
        cv::putText(frame_hsv, std::to_string(h), {10, 20}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::putText(frame_hsv, std::to_string(s), {10, 40}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::putText(frame_hsv, std::to_string(v), {10, 60}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::putText(frame_hsv, std::to_string(high_h), {10, 80}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::putText(frame_hsv, std::to_string(high_s), {10, 100}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::putText(frame_hsv, std::to_string(high_v), {10, 120}, cv::FONT_HERSHEY_PLAIN, 1.0, {0, 255, 0, 255});
        cv::imshow("HSV", frame_hsv);

        char pressed_key = cv::waitKey(2);
        if (pressed_key == 'x') {
            cv::Rect2d selection = cv::selectROI("HSV", frame_hsv);
            cv::imwrite("saved.jpg", cv::Mat(frame_hsv, selection));
        }
        else if (pressed_key == 27) {
            break;
        }

    } while (true);

    return 0;
}