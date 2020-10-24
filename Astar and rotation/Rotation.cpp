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
	int th;
};

struct Compare
{
	bool operator()(node1 const&p1,node1 const&p2)
	{
		return ((p1.f_value)>(p2.f_value));
	}

};

priority_queue<node1,vector<node1>, Compare> pq1;

//int check(int a,int b,Mat img);
float dist(int x1,int y1,int x2,int y2);
int rot(int th,int x,int y,Mat img);
void astar(struct node1 ini,struct node1 fin,Mat pop,Mat par,Mat img,Mat rota,int r,int c);
void path(Mat imgc,Mat par,Mat rota,struct node1 ini,struct node1 fin);

int main()
{
	Mat img=imread("Test2.png",0);
	Mat img2=imread("Test2.png",1);

	int r=img.rows;
	int c=img.cols;
	cout<<r<<endl<<c<<endl;
	Mat pop(r,c,CV_8UC1,Scalar(0));
	Mat par(r,c,CV_8UC1,Scalar(0));
	Mat rota(r,c,CV_8UC1,Scalar(0));

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

	astar(ini,fin,pop,par,img,rota,r,c);

	path(img2,par,rota,ini,fin);
				

return 0;
}

void astar(struct node1 ini,struct node1 fin,Mat pop,Mat par,Mat img,Mat rota,int r,int c)
{
	priority_queue<struct node1,vector<node1>,struct Compare> q;
	q.push(ini);
	struct node1 a;
	int i,j;
	float r0=sqrt(2);
	int t;

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
				{if((pop.at<uchar>(i,j+1)==0)&&(img.at<uchar>(i,j+1)==0))
				{
						a.th=180;	
						a.x=i;
						a.y=j+1;
						a.p=5;
						a.g_value=q.top().g_value+1;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(180,i,j,img))	q.push(a);

				}}	
				if((j-1)>=0)	
				{if((pop.at<uchar>(i,j-1)==0)&&(img.at<uchar>(i,j-1)==0))
				{
						a.th=0;	
						a.x=i;
						a.y=j-1;
						a.p=4;
						a.g_value=q.top().g_value+1;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(0,i,j,img)) q.push(a);
									
				}}
				if((i+1)<r)
				{if((pop.at<uchar>(i+1,j)==0)&&(img.at<uchar>(i+1,j)==0))
				{
				
						a.th=90;
						a.x=i+1;
						a.y=j;
						a.p=7;
						a.g_value=q.top().g_value+1;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(90,i,j,img))	q.push(a);
									
				}}
				if((i-1)>=0)
				{if((pop.at<uchar>(i-1,j)==0)&&(img.at<uchar>(i-1,j)==0))
				{
			
						a.th=90;
						a.x=i-1;
						a.y=j;
						a.p=2;
						a.g_value=q.top().g_value+1;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);;
						if(rot(90,i,j,img))	q.push(a);

				}}
				if((i-1)>=0&&(j-1)>=0)
				{if((pop.at<uchar>(i-1,j-1)==0)&&(img.at<uchar>(i-1,j-1)==0))
				{
					
						a.th=135;
						a.x=i-1;
						a.y=j-1;
						a.p=1;
						a.g_value=q.top().g_value+r0;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(135,i,j,img))	q.push(a);

				}}
				if((i-1)>=0&&(j+1)<c)
				{if((pop.at<uchar>(i-1,j+1)==0)&&(img.at<uchar>(i-1,j+1)==0))
				{
					
						a.th=45;
						a.x=i-1;
						a.y=j+1;
						a.p=3;
						a.g_value=q.top().g_value+r0;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(45,i,j,img))	q.push(a);

				}}
				if((i+1)<r&&(j-1)>=0)
				{if((pop.at<uchar>(i+1,j-1)==0)&&(img.at<uchar>(i+1,j-1)==0))
				{
					
						a.th=45;
						a.x=i+1;
						a.y=j-1;
						a.p=6;
						a.g_value=q.top().g_value+r0;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(45,i,j,img))	q.push(a);

				}}
				if((i+1)<r&&(j+1)<c)
				{if((pop.at<uchar>(i+1,j+1)==0)&&(img.at<uchar>(i+1,j+1)==0))
				{	
						
						a.th=135;
						a.x=i+1;
						a.y=j+1;
						a.p=8;
						a.g_value=q.top().g_value+r0;
						a.f_value = a.g_value+dist(a.x,a.y,fin.x,fin.y);
						if(rot(135,i,j,img))	q.push(a);
									
				}}
				pop.at<uchar>(i,j)=255;
				par.at<uchar>(i,j)=q.top().p;
				rota.at<uchar>(i,j)=q.top().th;
				//cout<<(int)par.at<uchar>(i,j);
				//cout<<q.top().cost<<" ";
				
		}	q.pop();	

	}
		
		pop.at<uchar>(q.top().x,q.top().y)=255;
		par.at<uchar>(q.top().x,q.top().y)=q.top().p;
		
	cout<<"END"<<endl<<q.top().x<<" "<<q.top().y<<endl;
}	

void path(Mat img,Mat par,Mat rota,struct node1 ini,struct node1 fin)
{
	int i,j;
	i=fin.x;
	j=fin.y;
	
	while(par.at<uchar>(i,j)!=0)
	{	
		int th=rota.at<uchar>(i,j);
		int r=img.rows;
		int c=img.cols;
		Mat imgc=img.clone();
	
		float t=th*3.14/180;
		for(int x=-botx/2;x<=botx/2;x++)
		{
			for(int y=-boty/2;y<=boty/2;y++)
			{
				imgc.at<Vec3b>(floor(i+x*cos(t)-y*sin(t)),floor(j+y*cos(t)+x*sin(t)))=255;
			}
		}
		namedWindow("W1",WINDOW_NORMAL);
		imshow("W1",imgc);
		waitKey(25);
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


int rot(int th,int x,int y,Mat img)
{
	
	int flg=1;
		float t=th*3.14/180;
		for(int i=-botx/2;i<=botx/2;i++)
		{
			if(i==-botx/2||i==botx/2)
			{
				for(int j=-boty/2;j<=boty/2;j++)
				{
					if(img.at<uchar>(floor(x+i*cos(t)-j*sin(t)),floor(y+j*cos(t)+i*sin(t)))==255)
					{
						flg=0;
					}
				}
			}	
				else
				{	
					int j=-boty/2;
					if(img.at<uchar>(floor(x+i*cos(t)-j*sin(t)),floor(y+j*cos(t)+i*sin(t)))==255)
					{
						flg=0;	
					}
					  j=boty/2;
					if(img.at<uchar>(floor(x+i*cos(t)-j*sin(t)),floor(y+j*cos(t)+i*sin(t)))==255)
					{
						flg=0;
					}
					
				}
		}
	
	return flg;	
}

float dist(int x1,int y1,int x2,int y2)
{
	return (hypot(x1-x2,y1-y2));
}