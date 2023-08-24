#pragma once
#include <opencv.hpp>
#include "model.h"

int getframe(cv::VideoCapture cap, cv::Mat& frame);
double delsame(cv::Mat lastframe, cv::Mat nowframe);
float ssim(cv::Mat im1, cv::Mat im2);
