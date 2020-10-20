#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/base.hpp>
#include <iostream>

int main( int argc, char**argv) {

    bool capturing = true;
    cv::VideoCapture cap(0);

    if ( !cap.isOpened() ) {
        std::cerr << "error opening frames source" << std::endl;
        return -1;
    }

    std::cout << "Video size: " << cap.get( cv::CAP_PROP_FRAME_WIDTH ) << "x" << cap.get( cv::CAP_PROP_FRAME_HEIGHT ) << std::endl;

    do {
        cv::Mat frame, frameFlip;
        if ( cap.read( frame ) ) {
            if(argc > 1) {
                if(strcmp(argv[1], "noflip") == 0) {
                    //Normal camera image
                    cv::imshow( "Not-yet smart windown", frame );
                }
                else {
                    std::cout << "Wrong argument! Exiting..." << std::endl;
                    return 0;
                }
            }
            else {
                //Flipped camera image
                cv::flip(frame, frameFlip, 1);
                cv::imshow( "Not-yet smart windown", frameFlip );
            }
        }
        else
            capturing = false;

        if( (cv::waitKey( 1000.0/60.0 )&0x0ff) == 27 ) capturing = false;

    } while( capturing );
    
    return 0;
}