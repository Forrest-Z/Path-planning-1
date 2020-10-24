#include"opencv2/core/core.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/highgui/highgui.hpp"
#include<iostream>
#include<cmath>
#include<bits/stdc++.h>
#include<queue>
#include<vector>
#include <functional>

using namespace cv;
using namespace std;

double va=1;
double wa=0.5;
double a1=1;
double b=0.2;
double y=1;
double n=5;
double t=0.15;


struct node1{
	
	int x;
	int y;
	double v;
	double w;
	double th;
	double obj;
};

double dist(int x1,int y1,int x2,int y2);
double heading(node1 a,int x,int y);
double clearance(node1 a,double v,double w,Mat img);
int main()
{
	Mat img=imread("Test1.png",0);
	Mat img2=imread("Test1.png",1);

	int r=img.rows;
	int c=img.cols;
	//cout<<r<<endl<<c<<endl;


		int grnx,grny,redx,redy,xi,yi,xf,yf;
	grnx=0;	grny=0;	redx=0;	redy=0;

	int flag =1;
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
			{
				if(img2.at<Vec3b>(i,j)[0]==0 && img2.at<Vec3b>(i,j)[1]==255)
					{
						if(flag){xi=i;	yi=j;	flag=0;}
						xf=i;	yf=j;
					}
			}
	}
	grnx=(xi+xf)/2;
	grny=(yi+yf)/2;

	flag =1;
	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
			{
				if(img2.at<Vec3b>(i,j)[0]==0 && img2.at<Vec3b>(i,j)[2]==255)
					{
						if(flag){xi=i;	yi=j;	flag=0;}
						xf=i;	yf=j;
					}
			}
	}
	redx=(xi+xf)/2;
	redy=(yi+yf)/2;
	
	struct node1 ini,fin;

	fin.x=redy;
	fin.y=redx;

	ini.x=grny;
	ini.y=grnx;
	ini.v=0;
	ini.w=0;
	ini.th=0;
	ini.th=heading(ini,fin.x,fin.y);

	
	Mat imgc=img2.clone();

//	img.at<uchar>(ini.x,ini.y)=255;
//	img.at<uchar>(fin.x,fin.y)=255;

	struct node1 main1;
	main1=ini;
	main1.obj=-100000;
	//cout<<main1.th;
	

	while(dist(main1.x,main1.y,fin.x,fin.y)!=0)
	{
		struct node1 max;
		max=main1;

		double vmin=0;
		if(main1.v>va)	vmin=main1.v-va;

		for(double v=main1.v+va ; v>vmin; v-=va/n)
		{
			for(double w=main1.w+wa ; w>main1.w-wa ; w-=wa/n)
			{	
			//double w=0;
				struct node1 a;

				if(w==0)	
				{
					a.x=main1.x + v*cos(a.th)*t;
					a.y=main1.y - v*sin(a.th)*t;
				}

				if(w!=0)
				{
					int r=(v/w);
					if(r<0)	r=-r;

					a.x=main1.x + r*(-sin(a.th)+sin(a.th+wa*t));
					a.y=main1.y + r*(-cos(a.th)+cos(a.th+wa*t));			
				}	

				if(a.x<0 || a.x>=c || a.y<0 || a.y>=r)	continue;

				a.th=main1.th + w*t;

				a.v=v;
				a.w=w;
				//double clear=clearance(a,v,w,imgc);
				double head=heading(a,fin.x,fin.y);
				//if(v>sqrt(2*va*clear))	continue;

					a.obj= -a1*head /*+ b*clear*/ + y*v;
					//cout<<a.x<<" "<<a.y<<" "<<-a1*head<<" "<<y*v<<" "<<b*clear<<" "<<a.obj<<endl;

					if(a.obj>max.obj)	max=a;
			}
	
		}
		if(main1.obj == max.obj)	max.v ++;
			main1=max;
			imgc.at<Vec3b>(main1.y,main1.x)[2]=255;
			namedWindow("W1",WINDOW_NORMAL);
			imshow("W1",imgc);
			waitKey(1000);
		cout<<main1.x<<" "<<main1.y<<" "<<heading(main1,fin.x,fin.y)<<" 	"<<main1.v<<"	 "<<main1.w<<"	 "/*<<clearance(main1,main1.v,main1.w,imgc)*/<<"  "<<main1.obj<<endl;
	}	



	
				
return 0;
}

double heading(node1 a,int x,int y)
{
	double x1;
	x1=-1*atan((y-a.y)/(x-a.x));

	if(x1>=a.th)
	return (x1-a.th);
	
	return (-x1+a.th);

}

double clearance(node1 a,double v,double w,Mat img)
{
	if(w!=0&&v!=0)
	{	
		int r =	v/w;
			if(r<0)	r=-r;

		int cx,cy;
		cx=(a.x-r*sin(a.th));
		cy=(a.y-r*cos(a.th));

			double min=10000;
			for(int i=0;i<img.cols;i++)
			{
				for(int j=0;j<img.rows;j++)
				{
					if(floor(dist(i,j,cx,cy))==r)
					{
						if(img.at<uchar>(j,i)==255)
						{
							double dis=dist(i,j,a.x,a.y);
							if(dis<min)	min=dis;
						}
					}
				}
			}

			if(min!=10000)	return 2*r*asin(min*1.0/(2*r));
			return 3.14*r;
	}	
	if(w==0)
	{
		for(int i=0;i<img.cols;i++)
		{
			for(int j=0;j<img.rows;j++)
			{
				if(i!=a.x)
				if((a.y-j)/(i-a.x)==tan(a.th))	
					if(img.at<uchar>(j,i)==255)	
						return dist(i,j,a.x,a.y);
			}
		}

		return (sqrt(img.rows*img.rows+img.cols*img.cols));
	}
return 0;
}


double dist(int x1,int y1,int x2,int y2)
{
	double x= sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	return (x);
}