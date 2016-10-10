#pragma once

#include <iostream>
#include <random>
#include "opencv2/opencv.hpp"

class cStoreCandy
{
private:

	struct sDesignes
	{
		cv::Mat oImg;
		cv::Mat oAlphaChannel;
	};
	
	struct sCandys
	{
		cv::Rect rctPosition;
		int nDesign;
	};

	struct sFallingCandys
	{
		cv::Rect rctPosition;
		int nDesign;
		double dNextPush;
		int nSpeed;
	};
	
	std::vector<sDesignes> moAvailableCandy;
	std::vector<sCandys> moCurrentCandy;
	std::vector<sFallingCandys> moCurrentFallingCandy;
	std::vector<cv::Point> moGrid;
	std::vector<cv::Point> moFalls;

	int mnScore;
	int mnCurPos;
	int mnHeight;
	int mnLost;
	int mnDesignCounter;
	double mdTimerStart;
	double mdLastChange;

public:

	cStoreCandy();
	~cStoreCandy();

	int initialize(std::vector<cv::Mat> oAvailableCandy, int nWidth, int nHeight);
	int setUpCandy(cv::Rect rctFacePosition);
	int setUpFallingCandy();
	int candyFall();
	int checkCollision(cv::Rect rctFacePosition);
	int plotCandy(cv::Mat * oImg);
	
};