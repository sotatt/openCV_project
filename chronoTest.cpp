#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main()
{
    // 画像読み込み（グレースケール）
    cv::Mat img = cv::imread("images/himawari.jpg", cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "画像が読み込めませんでした。" << endl;
        return -1;
    }

    cv::Mat img_canny, img_lap, img_sobel_x, img_sobel_y, img_sobel;

    // --------------------------------
    // ① Canny法（時間計測）
    // --------------------------------
    auto start_canny = high_resolution_clock::now();
    cv::Canny(img, img_canny, 125, 255);
    auto end_canny = high_resolution_clock::now();
    double time_canny = duration<double, milli>(end_canny - start_canny).count();

    // --------------------------------
    // ② Laplacian法（時間計測）
    // --------------------------------
    auto start_lap = high_resolution_clock::now();
    cv::Laplacian(img, img_lap, CV_16S, 3);
    cv::convertScaleAbs(img_lap, img_lap);
    cv::threshold(img_lap, img_lap, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    auto end_lap = high_resolution_clock::now();
    double time_lap = duration<double, milli>(end_lap - start_lap).count();

    // --------------------------------
    // ③ Sobel法（時間計測）
    // --------------------------------
    auto start_sobel = high_resolution_clock::now();

    cv::Sobel(img, img_sobel_x, CV_16S, 1, 0, 3);
    cv::Sobel(img, img_sobel_y, CV_16S, 0, 1, 3);

    cv::Mat sobel_abs_x, sobel_abs_y;
    cv::convertScaleAbs(img_sobel_x, sobel_abs_x);
    cv::convertScaleAbs(img_sobel_y, sobel_abs_y);

    img_sobel = sobel_abs_x + sobel_abs_y;
    cv::threshold(img_sobel, img_sobel, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    auto end_sobel = high_resolution_clock::now();
    double time_sobel = duration<double, milli>(end_sobel - start_sobel).count();

    cv::Mat noise(img.size(), CV_16SC1);

    // --------------------------------
    // 結果の表示
    // --------------------------------
    cout << "使用写真: moai" << endl;
    cout << "=== 処理時間（ms） ===" << endl;
    cout << "Canny     : " << time_canny << " ms" << endl;
    cout << "Laplacian : " << time_lap << " ms" << endl;
    cout << "Sobel     : " << time_sobel << " ms" << endl;
    

    // --------------------------------
    // 画像保存
    // --------------------------------
    cv::imwrite("result_canny.jpg", img_canny);
    cv::imwrite("result_laplacian.jpg", img_lap);
    cv::imwrite("result_sobel.jpg", img_sobel);

    // --------------------------------
    // 画像表示
    // --------------------------------
    cv::imshow("Original", img);
    cv::imshow("Canny", img_canny);
    cv::imshow("Laplacian", img_lap);
    cv::imshow("Sobel", img_sobel);

    // // ガウシアンノイズの付加
    // double sigma = 10.0;

    // cv::randn(noise, 0, sigma);  // 平均0、標準偏差 sigma
    // cv::Mat img_noise;
    // img.convertTo(img_noise, CV_16SC1);
    // img_noise = img_noise + noise;

    // // 画素値を0〜255に制限して8bitに戻す
    // cv::Mat gaussian_img;
    // cv::convertScaleAbs(img_noise, gaussian_img);
    // cout << "ガウシアンノイズの付与実験" << endl;
    // cv::imwrite("result_gaussian_noise_himawari.jpg", gaussian_img);
    // cv::imshow("Gaussian Noise", gaussian_img);

    cv::waitKey(0);
    return 0;
}