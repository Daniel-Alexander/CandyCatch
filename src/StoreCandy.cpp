#include "StoreCandy.h"

using namespace std;
using namespace cv;

cStoreCandy::cStoreCandy()
{
	mnScore = 0;
	mdTimerStart = (double)getTickCount();
	mdLastChange = 0;
	mnCurPos = 0;
	mnHeight = 0;
	mnLost = 0;
	mnDesignCounter = 0;
}

cStoreCandy::~cStoreCandy()
{
}

int cStoreCandy::initialize(std::vector<cv::Mat> oAvailableCandy, int nWidth, int nHeight)
{
	if( (oAvailableCandy.size() <= 0) ||
		(nWidth < 30) ||
		(nHeight < 30) )
	{
		cout << "Bad Initialzation of Store Candy Class.. STOP" << endl;
		return -1;
	}
	mnHeight = nHeight;

	// Set Up Candys
	for (size_t i = 0; i < oAvailableCandy.size(); i++)
	{
		resize(oAvailableCandy.at(i), oAvailableCandy.at(i), Size(20, 20));
		sDesignes curDesign;
		vector<Mat> channels;
		cvtColor(oAvailableCandy.at(i), curDesign.oImg, CV_RGBA2RGB);
		split(oAvailableCandy.at(i), channels);
		curDesign.oAlphaChannel = channels[3];
		moAvailableCandy.push_back(curDesign);
	}

	if (moAvailableCandy.size() <= 0)
	{
		cout << "Failed setting up Designes.. STOP" << endl;
		return -1;
	}
	
	// Compute Grid
	int nTrialsX = 7;
	int nTrialsY = 7;
	
	int nStepX = nWidth/nTrialsX;
	int nStepY = nHeight/nTrialsY;

	Point pntCurrent;
	
	for(int i=1; i < nTrialsX; i++)
	{
		for(int j=1; j < nTrialsY; j++)
		{
			pntCurrent.x = i*nStepX;
			pntCurrent.y = j*nStepY;
			
			moGrid.push_back(pntCurrent);
		}

		if (i != (nTrialsX - 1))
		{
			pntCurrent.x += (int)(nStepX / 2);
			pntCurrent.y = 10;

			moFalls.push_back(pntCurrent);
		}
	}
	std::cout << moFalls.size() << std::endl;
	return 0;	
}

int cStoreCandy::setUpCandy(cv::Rect rctFacePosition)
{
	if(moCurrentCandy.size() >= 10)
	{
		cout << "Already enough Candy out there.. Doing Nothing" << endl;
		return -1;
	}
	
	std::random_device rd;
	std::default_random_engine posGenerator(rd());
	std::uniform_int_distribution<int> distr(1, 35);
	int curPos = distr(posGenerator);
	
	
	if (rctFacePosition.x < moGrid.at(curPos).x + 10 &&
		rctFacePosition.x + rctFacePosition.width > moGrid.at(curPos).x &&
		rctFacePosition.y < moGrid.at(curPos).y + 10 &&
		rctFacePosition.height + rctFacePosition.y > moGrid.at(curPos).y)
	{
		// FIXME distribute positions in this case more dynamik
		curPos++;
		if (curPos > 35) curPos = 0;
	}
	
	sCandys candy;
	candy.rctPosition = Rect(moGrid.at(curPos).x - 10, moGrid.at(curPos).y - 10, 20, 20);

	candy.nDesign = mnDesignCounter;
	if (mnDesignCounter >= (moAvailableCandy.size() - 1) )
	{
		mnDesignCounter = 0;
	}
	else
	{
		mnDesignCounter++;
	}
	moCurrentCandy.push_back(candy);
	
	return 0;
}

int cStoreCandy::setUpFallingCandy()
{
	if (moCurrentFallingCandy.size() >= moFalls.size())
	{
		std::cout << "All are Falling" << std::endl;
		return 0;
	}

	std::random_device rd;
	std::default_random_engine posGenerator(rd());
	std::uniform_int_distribution<int> distr(1, (moFalls.size()-1));
	int curPos = distr(posGenerator);

	sFallingCandys candy;

	candy.rctPosition = Rect(moFalls.at(curPos).x - 10, moFalls.at(curPos).y - 10, 20, 20);

	// TODO make this more playable
	candy.dNextPush = 0;
	candy.nSpeed = 5;
	candy.nDesign = mnDesignCounter;
	if (mnDesignCounter >= (moAvailableCandy.size() - 1))
	{
		mnDesignCounter = 0;
	}
	else
	{
		mnDesignCounter++;
	}

	moCurrentFallingCandy.push_back(candy);

	return 0;

}

int cStoreCandy::candyFall()
{
	int nLoss = 0;
	if (moCurrentFallingCandy.size() < 1) return 0;

	double end = (double)cv::getTickCount();
	double seconds = (end - mdTimerStart) / cv::getTickFrequency();

	for (size_t i = 0; i < moCurrentFallingCandy.size(); i++)
	{
		if (seconds > moCurrentFallingCandy.at(i).dNextPush)
		{
			if ((moCurrentFallingCandy.at(i).rctPosition.y + moCurrentFallingCandy.at(i).rctPosition.height) < mnHeight)
			{
				moCurrentFallingCandy.at(i).rctPosition.y += moCurrentFallingCandy.at(i).nSpeed;
				moCurrentFallingCandy.at(i).dNextPush += .01;
			}
			else
			{
				moCurrentFallingCandy.erase(moCurrentFallingCandy.begin() + i);
				mnLost++;
				nLoss = -1;
			}
		}
	}

	return nLoss;
}

int cStoreCandy::checkCollision(cv::Rect rctFacePosition)
{
	int nCollide = candyFall();

	for (size_t i = 0; i < moCurrentCandy.size(); i++)
	{
		Rect candy = moCurrentCandy.at(i).rctPosition;

		if (rctFacePosition.x < candy.x + candy.width &&
			rctFacePosition.x + rctFacePosition.width > candy.x &&
			rctFacePosition.y < candy.y + candy.height &&
			rctFacePosition.height + rctFacePosition.y > candy.y)
		{
			mnScore++;
			mnCurPos = -1;
			moCurrentCandy.erase(moCurrentCandy.begin() + i);
			setUpCandy(rctFacePosition);
			nCollide = 1;
		}
	}

	for (size_t i = 0; i < moCurrentFallingCandy.size(); i++)
	{
		Rect candy = moCurrentFallingCandy.at(i).rctPosition;

		if (rctFacePosition.x < candy.x + candy.width &&
			rctFacePosition.x + rctFacePosition.width > candy.x &&
			rctFacePosition.y < candy.y + candy.height &&
			rctFacePosition.height + rctFacePosition.y > candy.y)
		{
			mnScore++;
			mnCurPos = -1;
			moCurrentFallingCandy.erase(moCurrentFallingCandy.begin() + i);
			nCollide = 1;
		}
	}

	return nCollide;
}

int cStoreCandy::plotCandy(cv::Mat * oImg)
{	
	double end = (double)cv::getTickCount();
	double seconds = (end - mdTimerStart) / cv::getTickFrequency();

	
	if ((mdLastChange + 2.0) < seconds)
	{
		setUpFallingCandy();
		//setUpCandy(Rect(1,1,1,1));
		mdLastChange = seconds;
	}
	

	for (size_t i = 0; i < moCurrentCandy.size();i++)
	{
		// TODO Replace by Candy Image
		// cv::rectangle(*oImg, Rect(moCurrentCandy.at(i).pntPosition.x - 10, moCurrentCandy.at(i).pntPosition.y - 10, 20, 20), cv::Scalar(0, 0255), CV_FILLED);
		//cv::rectangle(*oImg, moCurrentCandy.at(i).rctPosition, cv::Scalar(0, 0255), CV_FILLED);
		moAvailableCandy.at(moCurrentCandy.at(i).nDesign).oImg.copyTo((*oImg)(Rect(moCurrentCandy.at(i).rctPosition.x, moCurrentCandy.at(i).rctPosition.y, moAvailableCandy.at(moCurrentCandy.at(i).nDesign).oImg.cols, moAvailableCandy.at(moCurrentCandy.at(i).nDesign).oImg.rows)), moAvailableCandy.at(moCurrentCandy.at(i).nDesign).oAlphaChannel);
	}

	for (size_t i = 0; i < moCurrentFallingCandy.size(); i++)
	{
		moAvailableCandy.at(moCurrentFallingCandy.at(i).nDesign).oImg.copyTo((*oImg)(Rect(moCurrentFallingCandy.at(i).rctPosition.x, moCurrentFallingCandy.at(i).rctPosition.y, moAvailableCandy.at(moCurrentFallingCandy.at(i).nDesign).oImg.cols, moAvailableCandy.at(moCurrentFallingCandy.at(i).nDesign).oImg.rows)), moAvailableCandy.at(moCurrentFallingCandy.at(i).nDesign).oAlphaChannel);
	}

	/*
	for (size_t i = 0; i < moGrid.size(); i++)
	{
		cv::circle(*oImg, moGrid.at(i), 10, cv::Scalar(255, 255, 255));
	}
	for (size_t i = 0; i < moFalls.size(); i++)
	{
		cv::circle(*oImg, moFalls.at(i), 10, cv::Scalar(255, 255, 255));
	}
	*/

	char mcTxtScore[50];
	sprintf_s(mcTxtScore, "You ate: %i You Loose: %i", mnScore, mnLost);
	cv::Point pnt;
	pnt.x = 20;
	pnt.y = 20;

	putText(*oImg, mcTxtScore, pnt, FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 0, 0),2);

	return 0;
}
