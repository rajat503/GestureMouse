#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>

using namespace cv;
using namespace std;

void mouseMove(int x, int y)
{
    Display *displayMain = XOpenDisplay(NULL);

    if(displayMain == NULL)
    {
        
        exit(EXIT_FAILURE);
    }

    XWarpPointer(displayMain, None, None, 0, 0, 0, 0, x, y);

    XCloseDisplay(displayMain);
}
/*
void segmentation(Mat orig_img)
{
	int rows=orig_img.rows;
	int cols=orig_img.cols;
	Mat img_gray;
	Mat img_output = Mat(rows, cols, CV_8UC3);
	uchar* p;
	uchar* q;
	uchar* r;
	int i, j;
	int ib,ig,ir;
	ib=orig_img.ptr<uchar>(0)[0];
	ig=orig_img.ptr<uchar>(0)[1];
	ir=orig_img.ptr<uchar>(0)[2];
	for(i=0;i<orig_img.rows; i++)
	{
		p=orig_img.ptr<uchar>(i);
		q=img_output.ptr<uchar>(i);
		for(j=0;j<orig_img.cols*orig_img.channels();j+=orig_img.channels())
		{
			if((p[j]==ib && p[j+1]==ig && p[j+2]==ir) || (p[j]==255 && p[j+1]==255 && p[j+2]==255)) 
			{
				q[j]=0;
				q[j+1]=0;
				q[j+2]=0;
			}
			else
			{
				q[j]=p[j];
				q[j+1]=p[j+1];
				q[j+2]=p[j+2];	
			}
		}		
	}
	cvtColor(img_output,img_gray, COLOR_BGR2GRAY);
	threshold(img_gray, img_gray, 0, 255, THRESH_BINARY);
	Mat element = getStructuringElement(0,Size(2*3+1,2*3+1),Point(3,3));
	for(i=0;i<3;i++)
	{
		erode(img_gray, img_gray, element);
	}
	for(i=0;i<3;i++)
	{
		dilate(img_gray, img_gray, element);
	}
	for(i=0;i<orig_img.rows; i++)
	{
		r=img_gray.ptr<uchar>(i);
		p=orig_img.ptr<uchar>(i);
		q=img_output.ptr<uchar>(i);
		for(j=0;j<img_gray.cols;j++)
		{
			if(r[j]==255) 
			{
				q[j*orig_img.channels()]=p[j*orig_img.channels()];
				q[j*orig_img.channels()+1]=p[j*orig_img.channels()+1];
				q[j*orig_img.channels()+1]=p[j*orig_img.channels()+1];
			}
			else
			{
				q[j*orig_img.channels()]=0;
				q[j*orig_img.channels()+1]=0;
				q[j*orig_img.channels()+2]=0;	
			}
		}		
	}
	imshow("Output3",img_output); 
	waitKey(0);
	imwrite("Output3.jpg", img_output);
} */

int main()
{
	VideoCapture cap = VideoCapture(0);
	if (!cap.isOpened())
	{
		std::cout << "Unable to open camera\n";
		return -1;
	}
	int i,co=0,x1,y1,x2,y2,t1,t2;
	Mat img;
	Mat hsvimg;
	Mat green;
	while (1)
	{
		cap >> img;
		inRange(img, Scalar(55, 110, 100), Scalar(65, 125, 125), green);
		Mat element = getStructuringElement(0,Size(2*3+1,2*3+1),Point(3,3));
		for(i=0;i<2;i++)
		{
			dilate(green, green, element);
		}
		/*for(i=0;i<3;i++)
		{
			erode(green, green, element);
		}*/
		Moments mu;
    	Point2f mc;
    	mu = moments(green);
    	mc = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
    	circle(img, Point(int(mc.x), int(mc.y)), 3, Scalar(0, 0, 255), -1, 8, 0);
    	if(co==0)
    	{
    		x1=int(mc.x);
    		x2=int(mc.x);
    		y1=int(mc.y);
    		y2=int(mc.y);
    	}
    	else
    	{
    		x1=int(mc.x);
    		y1=int(mc.y);
    		if(abs(x2-x1)>5 || abs(y1-y2)>5)
    		{
    			mouseMove((x2-x1)*3,(y1-y2)*3);
    		}
    		
    	}
    	co++;
    	x2=x1;
    	y2=y1;
		imshow("input", img);
		imshow("video", green);
		int k = waitKey(60);
		if (k == 27) 
		{
			break;
		}
	}
	return 0;
}