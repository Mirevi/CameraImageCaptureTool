//#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/opencv.hpp>   // Include OpenCV API
// BETTER CAMERA CALIB
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <rs.hpp>

//using namespace cv;
#include <stdio.h>

//#include <opencv2/aruco.hpp>
#include <string>
#include <map>
#include <algorithm>
#include <mutex>                    // std::mutex, std::lock_guard
#include <cmath>

// ################################################################################
// ########################### Variables to set ###################################
// ################################################################################
/*
	Width, height and fps of the capture device
*/
int g_width = 1920;
int g_height = 1080;
int g_fps = 30;

/*
	The index of the device to use (necessary for VideoInput and OpenCV)
	default is 0
*/
int g_captureDeviceID = 0;

/*
	Set the path where the images should be stored on disk
*/
std::string g_imagePath = "/img/";
std::string g_imageExt = ".png";

int main(int argc, char* argv[])
{
	std::vector<std::string> fn;
	cv::VideoCapture cap;
	cv::Mat frame;
	rs2::context rs2ctx;
	rs2::pipeline rsPipe(rs2ctx);
	unsigned int imageCount = 0;
	
	{
		unsigned int i = 0;
		for (rs2::device&& device : rs2ctx.query_devices()) {
			if (i != g_captureDeviceID) continue;
			const char* serial = device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
			rs2::config rs2cfg;
			rs2cfg.enable_stream(RS2_STREAM_COLOR, -1, g_width, g_height, rs2_format::RS2_FORMAT_BGR8, g_fps);
			rs2cfg.enable_device(serial);
			rsPipe.start(rs2cfg);
			std::cout << "RealSense Device: " << serial << " started." << std::endl;
		}
	}

	bool liveCapture = true;
	while (true) {
		rs2::frameset frames = rsPipe.wait_for_frames();
		rs2::video_frame colorFrame = frames.get_color_frame();
		frame = cv::Mat(cv::Size(g_width, g_height), CV_8UC3, (void*) colorFrame.get_data(), cv::Mat::AUTO_STEP);
		//cv::Mat inputImageBackup = inputImage.clone();

		// Update the window with the new data
		imshow("Camera image", frame);
		int key = cv::waitKeyEx(liveCapture);
		if (key == 112) liveCapture = !liveCapture; // P
		else if (key == 32) { // SPACE
			std::string imageName = g_imagePath + std::to_string(imageCount) + g_imageExt;
			imwrite(imageName, frame);
			std::cout << imageCount++ << ": image has been saved at: " << imageName << std::endl;
		} else if (key == 27) break; // ESC
	}
	rsPipe.stop();
	return EXIT_SUCCESS;
}
