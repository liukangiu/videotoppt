#include "model.h"

using namespace cv;

double delsame(Mat  inputimage1, Mat inputimage2)
{
    double prob;
    if (inputimage1.type() == CV_8UC3)
    {
        cv::Size dsize(512, 512);
        cv::resize(inputimage1, inputimage1, dsize);
        cv::resize(inputimage2, inputimage2, dsize);
        Mat i1 = inputimage1;
        Mat i2 = inputimage2;
        const double C1 = 6.5025, C2 = 58.5225;
        int d = CV_32F;
        Mat I1, I2;
        i1.convertTo(I1, d);
        i2.convertTo(I2, d);
        Mat I2_2 = I2.mul(I2);
        Mat I1_2 = I1.mul(I1);
        Mat I1_I2 = I1.mul(I2);
        Mat mu1, mu2;
        GaussianBlur(I1, mu1, Size(11, 11), 1.5);
        GaussianBlur(I2, mu2, Size(11, 11), 1.5);
        Mat mu1_2 = mu1.mul(mu1);
        Mat mu2_2 = mu2.mul(mu2);
        Mat mu1_mu2 = mu1.mul(mu2);
        Mat sigma1_2, sigma2_2, sigma12;
        GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
        sigma1_2 -= mu1_2;
        GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
        sigma2_2 -= mu2_2;
        GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
        sigma12 -= mu1_mu2;
        Mat t1, t2, t3;
        t1 = 2 * mu1_mu2 + C1;
        t2 = 2 * sigma12 + C2;
        t3 = t1.mul(t2);
        t1 = mu1_2 + mu2_2 + C1;
        t2 = sigma1_2 + sigma2_2 + C2;
        t1 = t1.mul(t2);
        Mat ssim_map;
        divide(t3, t1, ssim_map);
        Scalar mssim = mean(ssim_map);
        prob = (mssim[0] + mssim[1] + mssim[2]) / 3;
    }
    if (inputimage1.type() == CV_8UC1)
    {
        cv::Size dsize(512, 512);
        cv::resize(inputimage1, inputimage1, dsize);
        cv::resize(inputimage2, inputimage2, dsize);
        cv::cvtColor(inputimage1, inputimage1, COLOR_BGR2GRAY);
        cv::cvtColor(inputimage2, inputimage2, COLOR_BGR2GRAY);
        Mat i1 = inputimage1;
        Mat i2 = inputimage2;
        const double C1 = 6.5025, C2 = 58.5225;
        int d = CV_32F;
        Mat I1, I2;
        i1.convertTo(I1, d);
        i2.convertTo(I2, d);
        Mat I2_2 = I2.mul(I2);
        Mat I1_2 = I1.mul(I1);
        Mat I1_I2 = I1.mul(I2);
        Mat mu1, mu2;
        GaussianBlur(I1, mu1, Size(11, 11), 1.5);
        GaussianBlur(I2, mu2, Size(11, 11), 1.5);
        Mat mu1_2 = mu1.mul(mu1);
        Mat mu2_2 = mu2.mul(mu2);
        Mat mu1_mu2 = mu1.mul(mu2);
        Mat sigma1_2, sigma2_2, sigma12;
        GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
        sigma1_2 -= mu1_2;
        GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
        sigma2_2 -= mu2_2;
        GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
        sigma12 -= mu1_mu2;
        Mat t1, t2, t3;
        t1 = 2 * mu1_mu2 + C1;
        t2 = 2 * sigma12 + C2;
        t3 = t1.mul(t2);
        t1 = mu1_2 + mu2_2 + C1;
        t2 = sigma1_2 + sigma2_2 + C2;
        t1 = t1.mul(t2);
        Mat ssim_map;
        divide(t3, t1, ssim_map);
        Scalar mssim = mean(ssim_map);
        prob = (mssim[0] + mssim[1] + mssim[2]) / 3;
    }

    //return mssim;
    return prob;
}

float ssim(Mat im1, Mat im2)
{
    int window = 7;
    float k1 = 0.01f;
    float k2 = 0.03f;
    float L = 255.f;
    CV_Assert(im1.size() == im2.size());
    cv::Size dsize(512, 512);
    cv::resize(im1, im1, dsize);
    cv::resize(im2, im2, dsize);
    cv::cvtColor(im1, im1, COLOR_BGR2GRAY);
    cv::cvtColor(im2, im2, COLOR_BGR2GRAY);
    im1.convertTo(im1, CV_32FC1);
    im2.convertTo(im2, CV_32FC1);
    int ndim = im1.dims;
    float NP = std::powf(window, ndim);
    float cov_norm = NP / (NP - 1);
    float C1 = (k1 * L) * (k1 * L);
    float C2 = (k2 * L) * (k2 * L);

    Mat ux, uy;
    Mat uxx = im1.mul(im1);
    Mat uyy = im2.mul(im2);
    Mat uxy = im1.mul(im2);

    blur(im1, ux, Size(window, window), Point(-1, -1));
    blur(im2, uy, Size(window, window), Point(-1, -1));

    blur(uxx, uxx, Size(window, window), Point(-1, -1));
    blur(uyy, uyy, Size(window, window), Point(-1, -1));
    blur(uxy, uxy, Size(window, window), Point(-1, -1));

    Mat ux_sq = ux.mul(ux);
    Mat uy_sq = uy.mul(uy);
    Mat uxy_m = ux.mul(uy);

    Mat vx = cov_norm * (uxx - ux_sq);
    Mat vy = cov_norm * (uyy - uy_sq);
    Mat vxy = cov_norm * (uxy - uxy_m);

    Mat A1 = 2 * uxy_m;
    Mat A2 = 2 * vxy;
    Mat B1 = ux_sq + uy_sq;
    Mat B2 = vx + vy;

    Mat ssim_map = (A1 + C1).mul(A2 + C2) / (B1 + C1).mul(B2 + C2);

    Scalar mssim = mean(ssim_map);
    //ssim_map.convertTo(ssim_map, CV_8UC1, 255, 0);

    //imshow("ssim", ssim_map);

    return mssim[0];
}