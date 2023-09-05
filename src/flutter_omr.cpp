#include <opencv2/opencv.hpp>
#include <chrono>

#ifdef __ANDROID__
#include <android/log.h>
#endif

using namespace cv;
using namespace std;

cv::Mat shadowRemove(const cv::Mat &img)
{
  std::vector<cv::Mat> rgbPlanes;
  cv::split(img, rgbPlanes);
  std::vector<cv::Mat> resultNormPlanes;

  for (const auto &plane : rgbPlanes)
  {
    cv::Mat dilatedImg;
    cv::dilate(plane, dilatedImg, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7)));

    cv::Mat bgImg;
    cv::medianBlur(dilatedImg, bgImg, 21);
    // cv::GaussianBlur(dilatedImg, bgImg, cv::Size(5, 5), 0);

    cv::Mat diffImg;
    cv::absdiff(plane, bgImg, diffImg);
    diffImg = 255 - diffImg;

    cv::Mat normImg;
    cv::normalize(diffImg, normImg, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    resultNormPlanes.push_back(normImg);
  }

  cv::Mat shadowRemov;
  cv::merge(resultNormPlanes, shadowRemov);

  return shadowRemov;
}

// CORNER POINTS
__attribute__((visibility("default"))) __attribute__((used))
std::vector<cv::Point>
getCornerPoints(const std::vector<cv::Point> &cont)
{
  double peri = cv::arcLength(cont, true);
  double epsilon = 0.02 * peri;
  std::vector<cv::Point> approx;
  cv::approxPolyDP(cont, approx, epsilon, true);
  return approx;
}

// FIND CONTOURS
__attribute__((visibility("default"))) __attribute__((used))
std::vector<std::vector<cv::Point>>
rectContour(const std::vector<std::vector<cv::Point>> &contours)
{
  std::vector<std::vector<cv::Point>> rectCon;
  for (const auto &contour : contours)
  {
    double area = cv::contourArea(contour);
    // platform_log("      limit:%lf    area in rectcontour: %lf", limit, area);
    if (area > 50)
    {
      //  platform_log("         area %lf > 50", area);
      double peri = cv::arcLength(contour, true);
      double epsilon = 0.02 * peri;
      std::vector<cv::Point> approx;
      cv::approxPolyDP(contour, approx, epsilon, true);
      if (approx.size() == 4)
      {
        rectCon.push_back(contour);
      }
    }
  }
  std::sort(rectCon.begin(), rectCon.end(), [](const std::vector<cv::Point> &a, const std::vector<cv::Point> &b)
            { return cv::contourArea(a) > cv::contourArea(b); });
  return rectCon;
}

__attribute__((visibility("default"))) __attribute__((used))
std::vector<cv::Point>
cornerFinder(cv::Mat &myMat, int answerCount, int questionCount)
{
  cv::Mat matGray;
  cv::Mat shadowRemoved = shadowRemove(myMat);
  cv::cvtColor(shadowRemoved, matGray, cv::COLOR_BGR2GRAY);
  cv::Mat matBlur;
  cv::GaussianBlur(matGray, matBlur, cv::Size(5, 5), 1);
  cv::Mat matThresh;
  cv::threshold(matBlur, matThresh, 0, 255,
                cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
  cv::Mat matCanny;
  cv::Canny(matThresh, matCanny, 10, 50);

  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;
  cv::findContours(matCanny, contours, hierarchy, cv::RETR_EXTERNAL,
                   cv::CHAIN_APPROX_NONE);
  // double limit = matCanny.rows / 30;
  std::vector<std::vector<cv::Point>> rectCon = rectContour(contours);
  int squareCount = 6;
  if (answerCount == 2 && questionCount <= 10)
  {
    squareCount = 5;
    if (rectCon.size() < 5)
      return std::vector<cv::Point>();
  }
  else if (rectCon.size() < 6)
  {
    // platform_log("insufficient contour amount - not enough rectangles: %d",
    // rectCon.size());
    return std::vector<cv::Point>();
  }
  // platform_log("rectcone.size: %d", rectCon.size());
  std::vector<std::vector<cv::Point>> squarecorners(6);

  squarecorners[0] = getCornerPoints(rectCon[0]);
  squarecorners[1] = getCornerPoints(rectCon[1]);
  squarecorners[2] = getCornerPoints(rectCon[2]);
  squarecorners[3] = getCornerPoints(rectCon[3]);
  squarecorners[4] = getCornerPoints(rectCon[4]);
  if (!(answerCount == 2 && questionCount <= 10))
    squarecorners[5] = getCornerPoints(rectCon[5]);

  cv::Point lefttop;
  cv::Point righttop;
  cv::Point leftbottom;
  cv::Point rightbottom;
  float minSum = squarecorners[0][0].x + squarecorners[0][0].y;
  float maxSum = squarecorners[0][0].x + squarecorners[0][0].y;
  int j = -1;
  int i;
  while (++j < squareCount)
  {
    i = -1;
    while (++i < 4)
    {
      float sum = squarecorners[j][i].x + squarecorners[j][i].y;
      if (sum <= minSum)
      {
        minSum = sum;
        lefttop.x = squarecorners[j][i].x;
        lefttop.y = squarecorners[j][i].y;
      }
      if (sum >= maxSum)
      {
        maxSum = sum;
        rightbottom.x = squarecorners[j][i].x;
        rightbottom.y = squarecorners[j][i].y;
      }
    }
  }
  float minDiv = squarecorners[0][0].x - squarecorners[0][0].y;
  float maxDiv = squarecorners[0][0].x - squarecorners[0][0].y;
  for (int j = 0; j < squareCount; ++j)
  {
    for (int i = 0; i < 4; ++i)
    {
      float div = squarecorners[j][i].x - squarecorners[j][i].y;
      if (div <= minDiv)
      {
        minDiv = div;
        leftbottom.x = squarecorners[j][i].x;
        leftbottom.y = squarecorners[j][i].y;
      }
      if (div >= maxDiv)
      {
        maxDiv = div;
        righttop.x = squarecorners[j][i].x;
        righttop.y = squarecorners[j][i].y;
      }
    }
  }

  std::vector<cv::Point> mycorners = {lefttop, righttop, leftbottom, rightbottom};
  return mycorners;
}

//
cv::Mat getBox(cv::Mat image, float boxWidth, float boxHeight, int c, int r);
std::vector<cv::Mat> splitBoxes(const cv::Mat &image, int questionCount, int choiceCount);

__attribute__((visibility("default"))) __attribute__((used)) cv::Mat
my_dilate(cv::Mat mat)
{
  cv::Mat result;
  cv::Mat kernel(3, 3, CV_8UC1, cv::Scalar(1));
  cv::dilate(mat, result, kernel);
  return result;
}

extern "C"
{

  void platform_log(const char *fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
#ifdef __ANDROID__
    __android_log_vprint(ANDROID_LOG_VERBOSE, "FFI Logger: ", fmt, args);
#else
    vprintf(fmt, args);
#endif
    va_end(args);
  }

  __attribute__((visibility("default"))) __attribute__((used)) const char *
  getOpenCVVersion()
  {
    return CV_VERSION;
  }

  __attribute__((visibility("default"))) __attribute__((used)) void
  convertImageToGrayImage(char *inputImagePath, char *outputPath)
  {
    platform_log("PATH %s: ", inputImagePath);
    cv::Mat img = cv::imread(inputImagePath);
    platform_log("Length: %d", img.rows);
    cv::Mat graymat;
    cvtColor(img, graymat, cv::COLOR_BGR2GRAY);
    cv::Mat blurmat;
    GaussianBlur(img, blurmat, Size(9, 9), 0);
    platform_log("Output Path: %s", outputPath);
    cv::imwrite(outputPath, blurmat);
    platform_log("Gray Image Length: %d", graymat.rows);
    platform_log("Image writed again ");
  }

  __attribute__((visibility("default"))) __attribute__((used)) void
  readOpticalForm(char *inputImagePath, char *outputPath, int questionCount,
                  int answerCount, int *resultBuffer)
  {

    platform_log("PATH %s: ", inputImagePath);
    cv::Mat img = cv::imread(inputImagePath);
    platform_log("cols: %d", img.cols);
    platform_log("Length: %d", img.rows);

    cv::Mat resized;
    cv::resize(img, resized, cv::Size(1150, 1550));

    //    CORNER FINDING
    std::vector<cv::Point> biggestContour =
        cornerFinder(resized, answerCount, questionCount);
    if (biggestContour.empty())
    {
      platform_log("Not enough contours");
      cv::Mat matGray;
      cv::Mat shadowRemoved = shadowRemove(resized);
      cv::cvtColor(shadowRemoved, matGray, cv::COLOR_BGR2GRAY);
      cv::Mat matBlur;
      cv::GaussianBlur(matGray, matBlur, cv::Size(5, 5), 1);
      cv::Mat matThresh;
      cv::threshold(matBlur, matThresh, 0, 255,
                    cv::THRESH_BINARY_INV + cv::THRESH_OTSU);
      platform_log("new image length: %d", matThresh.rows);
      cv::imwrite(outputPath, matThresh);
      platform_log("Image written again ");
      for (int i = 0; i < questionCount; i++)
        resultBuffer[i] = 999;
      return;
    }

    //    WARPING THE IMAGE
    cv::Mat matWarpColored;
    cv::Mat pt1(4, 2, CV_32F);
    for (int i = 0; i < 4; ++i)
    {
      pt1.at<float>(i, 0) = biggestContour[i].x;
      pt1.at<float>(i, 1) = biggestContour[i].y;
    }
    cv::Mat pt2 = (cv::Mat_<float>(4, 2) << 0, 0, 450, 0, 0, 750, 450, 750);
    cv::Mat matrix = cv::getPerspectiveTransform(pt1, pt2);
    cv::Mat matBiggestContours = resized.clone();
    cv::warpPerspective(matBiggestContours, matWarpColored, matrix,
                        cv::Size(450, 750));

    cv::Mat matWarpGray;
    cv::cvtColor(matWarpColored, matWarpGray, cv::COLOR_BGR2GRAY);

    cv::Mat matBlur;
    cv::GaussianBlur(matWarpGray, matBlur, cv::Size(5, 5), 0);

    cv::Mat matThresh;
    cv::threshold(matBlur, matThresh, 0, 255,
                  cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    //    SPLITTING THE IMAGE
    std::vector<cv::Mat> boxes =
        splitBoxes(matThresh, questionCount, answerCount);

    std::vector<std::vector<int>> pixelValues(questionCount,
                                              std::vector<int>(answerCount));
    int countC = 0;
    int countR = 0;
    int boxesTotal = 0;
    for (int i = 0; i < questionCount * answerCount; i++)
    {
      int totalPixels = cv::countNonZero(boxes[i]);
      pixelValues[countR][countC] = totalPixels;
      platform_log("totalPixels:%i i:%i letter:%i allpixels:%i\n", totalPixels, i,
                   i % 5, boxes[i].total());
      boxesTotal += totalPixels;
      countC++;
      if (countC == answerCount)
      {
        countR++;
        countC = 0;
      }
    }

    //    FINDING MARKED CHOICES
    std::vector<int> markings(questionCount);
    int limit = boxesTotal / (questionCount * (answerCount - 1));
    platform_log("total:%d limit: %d ", boxesTotal, limit);
    if (answerCount == 2)
      limit = boxesTotal / (questionCount * answerCount);
    platform_log("total:%d limit: %d ", boxesTotal, limit);
    for (int x = 0; x < questionCount; x++)
    {
      const std::vector<int> &arr = pixelValues[x];
      int maxIndex = 6;
      for (int i = 0; i < answerCount; i++)
      {
        if (arr[i] > limit)
        {
          if (maxIndex == 6)
          {
            maxIndex = i;
          }
          else
          {
            maxIndex = 7;
          }
        }
      }
      markings[x] = maxIndex;
    }

    platform_log("markings");
    for (const int &marking : markings)
    {
      platform_log("%d ", marking);
    }

    platform_log("Output Path: %s", outputPath);
    cv::imwrite(outputPath, matThresh);
    platform_log("resized Length: %d", resized.rows);

    for (int i = 0; i < questionCount; i++)
      resultBuffer[i] = markings[i];
  }
}

__attribute__((visibility("default"))) __attribute__((used))
std::vector<cv::Mat>
splitBoxes(const cv::Mat &image, int questionCount, int choiceCount)
{
  std::vector<cv::Mat> boxes;
  if (questionCount <= 10)
  {
    int numRows = questionCount + 6;
    int numCols = choiceCount + 2;
    float boxWidth = static_cast<float>(image.cols) / numCols;
    float boxHeight = static_cast<float>(image.rows) / numRows;
    for (int r = 0; r < numRows; ++r)
    {
      for (int c = 0; c < numCols; ++c)
      {
        if ((r == 0 && c == 0) || (r == 0 && c == choiceCount + 1) ||
            (r == questionCount + 5 && c == 0) ||
            (r == questionCount + 5 && c == choiceCount + 1))
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          int totalPixels = cv::countNonZero(roi);
          platform_log(">>>>>>totalPixels:%i allpixels:%i r:%i c:%i \n",
                       totalPixels, roi.total(), r, c);
        }
        if (r >= 5 && r < 5 + questionCount && c >= 2 && c < 2 + choiceCount)
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          boxes.push_back(roi);
        }
      }
    }
  }
  else if (questionCount > 10 && questionCount <= 20)
  {
    int numRows = 10 + 6;
    int numCols = choiceCount * 2 + 4;
    float boxWidth = static_cast<float>(image.cols) / numCols;
    float boxHeight = static_cast<float>(image.rows) / numRows;
    for (int r = 0; r < numRows; ++r) // first half
    {
      for (int c = 0; c < numCols; ++c)
      {
        if ((r == 0 && c == 0) || (r == 0 && c == choiceCount * 2 + 3) ||
            (r == 10 + 5 && c == 0) ||
            (r == 10 + 5 && c == choiceCount * 2 + 3))
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          int totalPixels = cv::countNonZero(roi);
          platform_log(">>>>>>totalPixels:%i allpixels:%i r:%i c:%i \n", totalPixels, roi.total(), r, c);
        }
        if (r >= 5 && r < 5 + 10 && c >= 2 && c < 2 + choiceCount)
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          boxes.push_back(roi);
        }
      }
    }
    for (int r = 0; r < numRows; ++r) // secondhalf
    {
      for (int c = 0; c < numCols; ++c)
      {
        if (r >= 5 && r < 5 + questionCount - 10 && c >= choiceCount + 4 &&
            c < 4 + choiceCount * 2)
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          boxes.push_back(roi);
        }
      }
    }
  }
  else
  {
    int numRows = questionCount / 2 + questionCount % 2 + 6;
    int numCols = choiceCount * 2 + 4;
    float boxWidth = static_cast<float>(image.cols) / numCols;
    float boxHeight = static_cast<float>(image.rows) / numRows;
    for (int r = 0; r < numRows; ++r) // first half
    {
      for (int c = 0; c < numCols; ++c)
      {
        if ((r == 0 && c == 0) || (r == 0 && c == numCols - 1) ||
            (r == numRows - 1 && c == 0) ||
            (r == numRows - 1 && c == numCols - 1))
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          int totalPixels = cv::countNonZero(roi);
          platform_log(">>>>>>totalPixels:%i allpixels:%i r:%i c:%i \n",
                       totalPixels, roi.total(), r, c);
        }
        if (r >= 5 && r < 5 + questionCount / 2 + questionCount % 2 && c >= 2 &&
            c < 2 + choiceCount)
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          boxes.push_back(roi);
        }
      }
    }
    for (int r = 0; r < numRows; ++r) // secondhalf
    {
      for (int c = 0; c < numCols; ++c)
      {
        if (r >= 5 && r < 5 + questionCount / 2 && c >= choiceCount + 4 &&
            c < 4 + choiceCount * 2)
        {
          cv::Mat roi = getBox(image, boxWidth, boxHeight, c, r);
          boxes.push_back(roi);
        }
      }
    }
  }
  return boxes;
}

__attribute__((visibility("default"))) __attribute__((used)) cv::Mat
getBox(cv::Mat image, float boxWidth, float boxHeight, int c, int r)
{
  float resize = 0.7;
  int startX = static_cast<int>(std::round(c * boxWidth));
  int startY = static_cast<int>(std::round(r * boxHeight));

  int width = static_cast<int>(std::round(boxWidth));
  int height = static_cast<int>(std::round(boxHeight));

  // to crop sides
  int smallerWidth = static_cast<int>(width * resize);
  int smallerHeight = static_cast<int>(height * resize);

  cv::Rect roiRect(startX, startY, width, height);
  if (roiRect.x < 0 || roiRect.y < 0 ||
      roiRect.x + roiRect.width > image.cols ||
      roiRect.y + roiRect.height > image.rows)
  {
    platform_log("ROI rect is out of bounds!    roiRect.x:%i roiRect.y:%i",
                 roiRect.x, roiRect.y);
    while (roiRect.x + roiRect.width > image.cols && roiRect.x != 0)
      roiRect.x--;
    while (roiRect.y + roiRect.height > image.rows && roiRect.y != 0)
      roiRect.y--;
    // if (roiRect.x == 0 || roiRect.y == 0)
    // {
    //     cv::Rect roiRect(c * boxWidth, r * boxHeight, boxWidth, boxHeight);
    //     return image(roiRect).clone();
    // }
    platform_log("roiRect.x:%i roiRect.y:%i", roiRect.x, roiRect.y);
  }
  cv::Mat roi = image(roiRect).clone();

  // calculate the amount to crop from each side of the ROI
  int cropWidth = static_cast<int>((width - smallerWidth) / 2);
  int cropHeight = static_cast<int>((height - smallerHeight) / 2);

  // perform cropping on the ROI to keep only the center portion
  cv::Rect cropRect(cropWidth, cropHeight, smallerWidth, smallerHeight);
  return (roi(cropRect).clone());
}