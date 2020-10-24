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

int botx=45;
int boty=75;

struct node1{
	int x;
	int y;
	int p;
	float g_value;
	float f_value;
};

struct Compare
{
	bool operator()(node1 const&p1,node1 const&p2)
	{
		return ((p1.f_value)>(p2.f_value));
	}

};

priority_queue<node1,vector<node1>, Compare> pq1;

float dist(int x1,int y1,int x2,int y2);
int check(int a,int b,Mat img);
void astar(struct node1 ini,struct node1 fin,Mat pop,Mat par,Mat img,int r,int c);
void path(Mat imgc,Mat par,struct node1 ini,struct node1 fin);

int main()
{
	Mat img=cv::imread("Test2.png",0);
	Mat img2=cv::imread("Test2.png",1);
	int r=img.rows;
	int c=img.cols;
	cout<<r<<endl<<c<<endl;
	Mat pop(r,c,CV_8UC1,Scalar(0));
	Mat par(r,c,CV_8UC1,Scalar(0));

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
	ini.x=redx;
	ini.y=redy;
	ini.g_value=0;
	ini.f_value=0;


	fin.x=grnx;
	fin.y=grny;

	for(int i=0;i<r;i++)
	{
		for(int j=0;j<c;j++)
			{
				if(img.at<uchar>(i,j)<250)
					img.at<uchar>(i,j)=0;
			}
	}

	astar(ini,fin,pop,par,img,r,c);

	path(img2,par,ini,fin);
				

return 0;
}

void astar(struct node1 ini,struct node1 fin,Mat pop,Mat par,Mat img,int r,int c)
{
	priority_queue<struct node1,vector<node1>,struct Compare> q;
	q.push(ini);
	struct node1 a;
	int i,j;
	float r0=sqrt(2);
	//namedWindow("W1",WINDOW_NORMAL);

	while((q.top().x)!=fin.x || (q.top().y)!=fin.y)
	{	
		/*namedWindow("W2",WINDOW_NORMAL);
		imshow("W2",pop);
		waitKey(1);*/
		if(q.empty())	{cout<<"No path exists"	;	exit(1);}
		i=(q.top()).x;
		j=(q.top()).y;
		if(pop.at<uchar>(i,j)==0&&img.at<uchar>(i,j)==0)	
		{	
				if((j+1)<c)
				{if((pop.at<uchar>(i,j+1)==0)&&(img.at<uchar>(i,j+1)==0)&&check(i,j+1,img))
				{
					a.x=i;
					a.y=j+1;
					a.p=5;
					a.g_value=q.top().g_value+1;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);

				}}	
				if((j-1)>=0)	
				{if((pop.at<uchar>(i,j-1)==0)&&(img.at<uchar>(i,j-1)==0)&&check(i,j-1,img))
				{
					a.x=i;
					a.y=j-1;
					a.p=4;
					a.g_value=q.top().g_value+1;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);	
									
				}}
				if((i+1)<r)
				{if((pop.at<uchar>(i+1,j)==0)&&(img.at<uchar>(i+1,j)==0)&&check(i+1,j,img))
				{
					a.x=i+1;
					a.y=j;
					a.p=7;
					a.g_value=q.top().g_value+1;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);
									
				}}
				if((i-1)>=0)
				{if((pop.at<uchar>(i-1,j)==0)&&(img.at<uchar>(i-1,j)==0)&&check(i-1,j,img))
				{
					a.x=i-1;
					a.y=j;
					a.p=2;
					a.g_value=q.top().g_value+1;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);

				}}
				if((i-1)>=0&&(j-1)>=0)
				{if((pop.at<uchar>(i-1,j-1)==0)&&(img.at<uchar>(i-1,j-1)==0)&&check(i-1,j-1,img))
				{
					a.x=i-1;
					a.y=j-1;
					a.p=1;
					a.g_value=q.top().g_value+r0;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);

				}}
				if((i-1)>=0&&(j+1)<c)
				{if((pop.at<uchar>(i-1,j+1)==0)&&(img.at<uchar>(i-1,j+1)==0)&&check(i-1,j+1,img))
				{
					a.x=i-1;
					a.y=j+1;
					a.p=3;
					a.g_value=q.top().g_value+r0;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);

				}}
				if((i+1)<r&&(j-1)>=0)
				{if((pop.at<uchar>(i+1,j-1)==0)&&(img.at<uchar>(i+1,j-1)==0)&&check(i+1,j-1,img))
				{
					a.x=i+1;
					a.y=j-1;
					a.p=6;
					a.g_value=q.top().g_value+r0;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);

				}}
				if((i+1)<r&&(j+1)<c)
				{if((pop.at<uchar>(i+1,j+1)==0)&&(img.at<uchar>(i+1,j+1)==0)&&check(i+1,j+1,img))
				{	
					a.x=i+1;
					a.y=j+1;
					a.p=8;
					a.g_value=q.top().g_value+r0;
					a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
					q.push(a);
									
				}}
				pop.at<uchar>(i,j)=255;
				par.at<uchar>(i,j)=q.top().p;
				//cout<<(int)par.at<uchar>(i,j);
				//cout<<q.top().g_value<<" ";
				
		}	q.pop();	
		//imshow("W1",pop);
		//waitKey(1);

	}
		
		pop.at<uchar>(q.top().x,q.top().y)=255;
		par.at<uchar>(q.top().x,q.top().y)=q.top().p;

		
	cout<<"END"<<endl<<q.top().x<<" "<<q.top().y<<endl;
}	

void path(Mat img2,Mat par,struct node1 ini,struct node1 fin)
{
	int i,j;
	i=fin.x;
	j=fin.y;
	namedWindow("W1",WINDOW_NORMAL);
	while(par.at<uchar>(i,j)!=0)
	{	
		int r=img2.rows;
		int c=img2.cols;
		Mat imgc=img2.clone();
		
			for(int a=-1*(botx/2);a<=botx/2;a++)
			{
				for(int b=-1*(boty/2);b<=boty/2;b++)
				{
					imgc.at<Vec3b>(a+i,b+j)=255;	
				}

			}

		imshow("W1",imgc);
		waitKey(15);
		if(par.at<uchar>(i,j)==1)
		{
			i=i+1;	j=j+1;
			continue;
		}
		if(par.at<uchar>(i,j)==2)
		{
			i=i+1;
			continue;
		}
		if(par.at<uchar>(i,j)==3)
		{
			i=i+1;	j=j-1;
			continue;
		}
		if(par.at<uchar>(i,j)==4)
		{
			j=j+1;
			continue;
		}
		if(par.at<uchar>(i,j)==5)
		{
			j=j-1;
			continue;
		}
		if(par.at<uchar>(i,j)==6)
		{
			i=i-1;	j=j+1;
			continue;
		}
		if(par.at<uchar>(i,j)==7)
		{
			i=i-1;
			continue;
		}
		if(par.at<uchar>(i,j)==8)
		{
			i=i-1;	j=j-1;
			continue;
		}
	}

}

int check(int a,int b,Mat img)
{
	for(int i=-1*(botx/2)-1;i<=botx/2+1;i++)
	{
		if(i==-1*(botx/2)-1||i==botx/2+1)
		{
			for(int j=-1*(boty/2)-1;j<=boty/2+1;j++)
			{
				if(img.at<uchar>(a+i,b+j)>0)
					return 0;
			}
		}

		else 
		{
			int j=-1*(botx/2)-1;
			if(img.at<uchar>(a+i,b+j)>0)
					return 0;

			j=1*(botx/2)+1;
			if(img.at<uchar>(a+i,b+j)>0)
					return 0;

		}

	}return 1;	

}

float dist(int x1,int y1,int x2,int y2)
{
	return (hypot(x1-x2,y1-y2));
}