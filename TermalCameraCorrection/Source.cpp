#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <vector>
#include <cmath>


using namespace std;
using namespace cv;

const int HEADER_BYTES_COUNT = 32;
const int FRAMES_COUNT = 1000;
const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 512;
const int BYTES_FOR_PIXEL = 2;
const int VIDEO_FPS = 30;

const uint16_t MAX_VALUE_16BIT = 65535; // Maximum value of 16-bit unsigned integer
const uint8_t MAX_VALUE_8BIT = 255; // Maximum value of 8-bit unsigned integer
const string INPUT_FILE_PATH = "D:/Download/dump_13122019_145433/dump_13122019_145433.bin";
const string OUTPUT_FILE_PATH = "D:/result.mp4";



// Calculate the average intensity of the input image
double calculate_average_intensity(const vector<uint16_t>& image) {
	int sum = 0;
	int num_pixels = image.size();

	for (int i = 0; i < num_pixels; i++) {
		sum += image[i];
	}
	return (double)sum / num_pixels;
}

// Calculate the standard deviation of the input image
double calculate_standard_deviation(const vector<uint16_t>& image, double mean) {
	double sum = 0;
	int num_pixels = image.size();

	for (int i = 0; i < num_pixels; i++) {
		sum += pow(image[i] - mean, 2);
	}
	return sqrt(sum / num_pixels);
}

// Find the gamma value for gamma correction
double find_gamma(const vector<uint16_t>& image) {
	double mean = calculate_average_intensity(image);
	double standard_deviation = calculate_standard_deviation(image, mean);
	double gamma = log10(0.5) / log10(mean + standard_deviation) - 1;
	return gamma;
}


// Convert 16-bit image to 8-bit image
void convert16to8(const vector<uint16_t>& src, vector<uint8_t>& dst)
{
	dst.resize(src.size());

	for (size_t i = 0; i < src.size(); i++)
	{
		uint16_t value16 = src[i];
		uint8_t value8 = static_cast<uint8_t>(MAX_VALUE_8BIT * 1.0 * value16 / MAX_VALUE_16BIT);
		dst[i] = value8;
	}
}


// Convert 1-channel image to 3-channel image
void gray2rgb(const vector<uint8_t>& gray_image, Mat& rgb_image)
{
	rgb_image.create(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);

	for (int i = 0; i < IMAGE_HEIGHT; i++)
	{
		for (int j = 0; j < IMAGE_WIDTH; j++)
		{
			uint8_t gray = gray_image[i * IMAGE_WIDTH + j];
			rgb_image.at<Vec3b>(i, j)[0] = gray;
			rgb_image.at<Vec3b>(i, j)[1] = gray;
			rgb_image.at<Vec3b>(i, j)[2] = gray;
		}
	}
}


// Function for gamma correction
void gamma_correction(vector<uint16_t>& image) {

	double gamma_value = find_gamma(image);

	// Apply gamma correction to each pixel
	for (auto& pixel : image) {
		double intensity = pow(pixel * 1.0 / MAX_VALUE_16BIT, 1.0 / gamma_value);
		pixel = static_cast<uint16_t>(intensity * MAX_VALUE_16BIT);
	}
}

void read_file(vector<vector<uint16_t>>& dst) {
	ifstream file(INPUT_FILE_PATH, ios::binary);

	assert(file.is_open());

	file.seekg(0, ios::end);
	int file_size = static_cast<int>(file.tellg()) - HEADER_BYTES_COUNT;
	assert(file_size == FRAMES_COUNT * IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_FOR_PIXEL);

	file.seekg(HEADER_BYTES_COUNT, ios::beg);
	for (int i = 0; i < FRAMES_COUNT; i++) {
		file.read(reinterpret_cast<char*>(dst[i].data()), IMAGE_WIDTH * IMAGE_HEIGHT * BYTES_FOR_PIXEL);
	}

	file.close();
}


int main()
{
	vector<vector<uint16_t>> images_16bit(FRAMES_COUNT, vector<uint16_t>(IMAGE_WIDTH * IMAGE_HEIGHT));
	vector<vector<uint8_t>> images_8bit(FRAMES_COUNT, vector<uint8_t>(IMAGE_WIDTH * IMAGE_HEIGHT));

	read_file(images_16bit);

	// Open a video file for writing
	VideoWriter video(OUTPUT_FILE_PATH, VideoWriter::fourcc('a', 'v', 'c', 1), VIDEO_FPS, Size(IMAGE_WIDTH, IMAGE_HEIGHT));

	// Check if the video file was opened successfully
	assert(video.isOpened());

	Mat rgb_image;
	for (int i = 0; i < FRAMES_COUNT; i++) {
		gamma_correction(images_16bit[i]);
		convert16to8(images_16bit[i], images_8bit[i]);
		gray2rgb(images_8bit[i], rgb_image);

		// Write the frame to the video file
		video.write(rgb_image);
	}

	// Release the video writer
	video.release();

	return 0;

}
