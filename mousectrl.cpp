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
void mouseClick(int button)
{

	Display *display = XOpenDisplay(NULL);
	XEvent event;
	if(display == NULL)
	{
		exit(EXIT_FAILURE);
	}
	memset(&event, 0x00, sizeof(event));
	event.type = ButtonPress;
	event.xbutton.button = Button3;
	event.xbutton.same_screen = True;
	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);	
	event.xbutton.subwindow = event.xbutton.window;
	while(event.xbutton.subwindow)
	{	
		event.xbutton.window = event.xbutton.subwindow;
		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}
	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
	XFlush(display);
	event.type = ButtonRelease;
	event.xbutton.state = 0x100;
	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) 
	XFlush(display);
	XCloseDisplay(display);
}
int main()
{
	VideoCapture cap = VideoCapture(0);
	if (!cap.isOpened())
	{
		std::cout << "Unable to open camera\n";
		return -1;
	}
	int i,co=0,x1,y1,x2,y2,co1=0;
	Mat img;
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
    		if(abs(x2-x1)>3 || abs(y1-y2)>3)
    		{
    			mouseMove((x2-x1)*3,(y1-y2)*3);
    			co1=0;
    		}
    		else
    		{
    			co1++;
    			if(co1==7)
    			{
    				mouseClick(0);
    				co1=0;
    			}
    		}	
    	}
    	co++;
    	x2=x1;
    	y2=y1;
		//imshow("input", img);
		//imshow("video", green);
		int k = waitKey(60);
		if (k == 27) 
		{
			break;
		}
	}
	return 0;
}