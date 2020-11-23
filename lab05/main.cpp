#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

CascadeClassifier plate_cascade;
vector<Rect> found_objects;
unsigned int total = 0;

void detectAndDisplay(Mat &frame)
{
    std::vector<Rect> plates;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    plate_cascade.detectMultiScale(frame_gray, plates, 1.1, 10, 0, Size(12, 12));

    std::cout << "Total number of objects: " << found_objects.size() << std::endl;

    for (size_t i = 0; i < plates.size(); i++)
    {
        Rect current_obj(plates[i].x, plates[i].y, plates[i].width, plates[i].height);

        Rect similar_object(-1, -1, 0, 0);

        if (found_objects.size() == 0)
        {
            found_objects.push_back(current_obj);
        }

        for (auto object : found_objects)
        {
            if (current_obj.x > (object.x - 25) && current_obj.x < (object.x + 25))
            {
                similar_object = object;
                break;
            }
            else if (current_obj.y > (object.y - 25) && current_obj.y < (object.y + 25))
            {
                similar_object = object;
                break;
            }
            else
                continue;
        }

        if (similar_object.x < 0)
        {
            found_objects.push_back(current_obj);
        }

        Mat plateROI = frame_gray(plates[i]);
        // imshow("ROI", plateROI);
        rectangle(frame, current_obj, {0, 255, 0}, 3);
    }

}

int main(void)
{
    VideoCapture capture;
    Mat frame;

    if (!plate_cascade.load("ru_plate.xml"))
        return -9;

    capture.open(0);
    if (!capture.isOpened())
        return -7;

    while (capture.read(frame))
    {
        if (frame.empty())
            return -1;

        detectAndDisplay(frame);

        cv::flip(frame, frame, 1);
        putText(frame, to_string(found_objects.size()), {30, 30}, FONT_HERSHEY_PLAIN, 3.0, {0, 255, 0});
        imshow("DWI", frame);

        if ((waitKey(1) & 0x0ff) == 27)
            return 0;
    }
    return 0;
}