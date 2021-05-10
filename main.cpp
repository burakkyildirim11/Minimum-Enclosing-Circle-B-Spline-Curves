#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <math.h>

#define maxPoint 1000


typedef struct
{
    float x,y; // x and y coordinates of the Circle1
    float r;   // radius of the Circle1

} Circle1;



void drawCoordinateSystem(Circle1);


float distanceTwoPoints(float,float,float,float); // The function that calculates the distance between two points

float bSplineFormula(float [],float,int,int); // B-spline basic function
void drawBspline(float [][2],int);   // B-spline curve drawing function

Circle1 comparePoint(float[][2],int);    // The function comparing the first two points for the minimum Circle1 calculation
Circle1 compareByPointGiven(float[][2],int,int);   // The function that calculates the point outside the Circle1 for the minimum Circle1 calculation
Circle1 compareByGivenTwoPoints(float[][2],int,int,int); // A function that calculates according to two points outside the Circle1 for the Minimum Circle1 calculation
Circle1 createMinCircle(float,float,float,float,float,float); // A function that creates a Circle1 according to three points outside the Circle1 for the minimum Circle1 calculation

int main()
{
    FILE *myFile;
    float pointCoordinates[maxPoint][2]; // Matrix of all points remaining in the Circle1

    if((myFile=fopen("data.txt","r"))==NULL)
    {
        printf("Error! Could not open file.");
    }
    else
    {
        int pointCount;
        fscanf(myFile, "%d", &pointCount);

        for(int i=0; i<pointCount; i++)
        {
            fscanf(myFile, "%f %f", &pointCoordinates[i][0],&pointCoordinates[i][1]);
        }

        Circle1 inclusiveCircle;
        inclusiveCircle=comparePoint(pointCoordinates,pointCount);

        initwindow( 840, 760, "Minimum Enclosing Circle1 & B-Spline",400,20);

        drawCoordinateSystem(inclusiveCircle); // The function that draws the coordinate system

        int originX=getmaxx()/2;
        int originY=getmaxy()/2;

        float xValue=(originX+inclusiveCircle.x*(float) 20);
        float yValue=(originY+inclusiveCircle.y*(float) -20);
        float radius=inclusiveCircle.r*(float) 20;
        setlinestyle(0,1,5);
        setcolor(3);
        circle(xValue+2,yValue+2,radius);


        setcolor(MAGENTA);
        for(int i=0; i<pointCount; i++)
        {
            arc(1+originX+pointCoordinates[i][0]*20,1+originY-pointCoordinates[i][1]*20,0,360,2);
            printf("{ %.2f,%.2f }\n",pointCoordinates[i][0],pointCoordinates[i][1]);
        }
        setcolor(4);
        arc(xValue,yValue,0,360,2);  //The center of the Circle1

        float x1,y1;
        x1=sin(45)*inclusiveCircle.r*20;
        y1=cos(45)*inclusiveCircle.r*20;
        setlinestyle(0,1,3);
        line(xValue,yValue,xValue+x1,yValue-y1); //radius line

        drawBspline(pointCoordinates,pointCount);

        getch();
    }
    return 0;
}
float distanceTwoPoints(float x0,float y0,float x1,float y1)
{
    float distance;
    distance=sqrt(pow(x0-x1, 2) + pow(y0 -y1, 2)); // formula that calculates the distance between two points

    return distance;
}
Circle1 comparePoint(float pointsMatrix[][2],int pointsCount1)
{
    Circle1 tempCircle;
    tempCircle.x = ((pointsMatrix[0][0]+pointsMatrix[1][0])/2);
    tempCircle.y = ((pointsMatrix[0][1]+pointsMatrix[1][1])/2);
    tempCircle.r = (distanceTwoPoints(pointsMatrix[0][0],pointsMatrix[0][1],pointsMatrix[1][0],pointsMatrix[1][1]))/2;

    int inside;
    for(int i=2; i<pointsCount1; i++)
    {
        inside = pow(pointsMatrix[i][0] - tempCircle.x, 2) + pow(pointsMatrix[i][1] - tempCircle.y, 2) - pow(tempCircle.r, 2);

        if(inside>0) //if the result is greater than zero, the given point is outside the Circle1
        {

            tempCircle=compareByPointGiven(pointsMatrix,i,pointsCount1);
        }
    }
    return tempCircle;
}
Circle1 compareByPointGiven(float pointsMatrix[][2],int outsidePointIndex,int pointsCount1)
{
    Circle1 tempCircle;
    tempCircle.x = ((pointsMatrix[0][0]+pointsMatrix[outsidePointIndex][0])/2);
    tempCircle.y = ((pointsMatrix[0][1]+pointsMatrix[outsidePointIndex][1])/2);
    tempCircle.r = distanceTwoPoints(pointsMatrix[0][0],pointsMatrix[0][1],pointsMatrix[outsidePointIndex][0],pointsMatrix[outsidePointIndex][1])/2;

    float inside;
    for(int i=1; i<outsidePointIndex; i++)
    {
        inside = pow(pointsMatrix[i][0] - tempCircle.x, 2) + pow(pointsMatrix[i][1] - tempCircle.y, 2) - pow(tempCircle.r, 2);

        if(inside>0)//if the result is greater than zero, the given point is outside the Circle1
        {
            tempCircle=compareByGivenTwoPoints(pointsMatrix,i,outsidePointIndex,i);
        }
    }
    return tempCircle;
}
Circle1 compareByGivenTwoPoints(float pointsMatrix[][2],int firstPointIndex,int secondPointIndex,int pointsCount1)
{
    Circle1 tempCircle;
    tempCircle.x = ((pointsMatrix[firstPointIndex][0]+pointsMatrix[secondPointIndex][0])/2);
    tempCircle.y = ((pointsMatrix[firstPointIndex][1]+pointsMatrix[secondPointIndex][1])/2);
    tempCircle.r = (distanceTwoPoints(pointsMatrix[firstPointIndex][0],pointsMatrix[firstPointIndex][1],pointsMatrix[secondPointIndex][0],pointsMatrix[secondPointIndex][1])/2);

    float inside;
    for(int i=0; i<pointsCount1; i++)
    {
        inside = pow(pointsMatrix[i][0] - tempCircle.x, 2) + pow(pointsMatrix[i][1] - tempCircle.y, 2) - pow(tempCircle.r, 2);

        if(inside>0)//if the result is greater than zero, the given point is outside the Circle1
        {
            tempCircle=createMinCircle(pointsMatrix[firstPointIndex][0],pointsMatrix[firstPointIndex][1],pointsMatrix[secondPointIndex][0],pointsMatrix[secondPointIndex][1],pointsMatrix[i][0],pointsMatrix[i][1]);
        }
    }
    return tempCircle;
}
Circle1 createMinCircle(float x0,float y0,float x1,float y1,float x2,float y2)    //a function that draws a Circle1 according to given three points
{
    float a,b,c,d,e,f,g,k,l,M,n,r,s,x,y,z;

    /*
    ax+by+cz+d=0 1. equation
    ex+my+nz+k=0 2. equation
    nx+ny+rz+s=0 3. equation
    */

    a=x0,b=y0,c=1.0; // coefficients of the first equation
    e=x1,f=y1,g=1.0; // coefficients of the second equation
    l=x2,n=y2,r=1.0; // coefficients of the third equation

    d=(x0*x0)+(y0*y0);
    s=(x2*x2)+(y2*y2);
    k=(x1*x1)+(y1*y1);

    M = ((a*f*r+b*l*g+c*e*n)-(a*g*n+b*e*r+c*f*l));  //The unknowns of the equation are found by the Cramer method
    x = ((b*r*k+c*f*s+d*g*n)-(b*g*s+c*n*k+d*f*r))/M;
    y = ((a*g*s+c*l*k+d*e*r)-(a*r*k+c*e*s+d*g*l))/M;
    z = ((a*n*k+b*e*s+d*f*l)-(a*f*s+b*l*k+d*e*n))/M;


    float xPoint,yPoint,radius;

    xPoint=x/2.0;
    yPoint=y/2.0;
    radius=(sqrt(pow(x,2)+pow(y,2)-4*z))/2.0;


    xPoint=xPoint*-1.0;
    yPoint=yPoint*-1.0;

    Circle1 minCircle;
    minCircle.x=xPoint;
    minCircle.y=yPoint;
    minCircle.r=radius;

    return minCircle;
}
float bSplineFormula(float knotArr[], float u, int i, int degree)
{

    if(degree==1)
    {
        if(knotArr[i]<=u && u<=knotArr[i+1])
            return 1;
        else
            return 0;
    }
    float result;
    result = ((u-knotArr[i])/(knotArr[i+degree-1]-knotArr[i])*bSplineFormula(knotArr, u, i, degree-1)) +
                ((knotArr[i+degree]-u)/(knotArr[i+degree]-knotArr[i+1])*bSplineFormula(knotArr, u, i+1, degree-1));
    return result;
}
void drawBspline(float pointsMatrix[][2],int pointsCount1)
{
    int pointCount,degree;
    degree=3;
    pointCount = pointsCount1;

    float knotArr[pointCount+degree];
    int i;
    for(i=0; i<pointCount+degree; i++)
    {
        knotArr[i]=(((double)i)/(pointCount+degree-1));
    }

    double x, y, basis, u;
    for(u=knotArr[degree-1]; u<=knotArr[pointCount]; u+=0.0001)
    {
        x = 0;
        y = 0;
        for(i=0;i<pointCount;i++)
        {
            basis = bSplineFormula(knotArr, u, i, degree);
            x += basis*pointsMatrix[i][0]*20.0;
            y -= basis*pointsMatrix[i][1]*20.0;
        }
        setlinestyle(0,0,3);
        putpixel(x+420.0,y+380.0, YELLOW);
    }
}
void drawCoordinateSystem(Circle1 inclusiveCircle)
{
    int originX=getmaxx()/2;
    int originY=getmaxy()/2;

    setcolor(8);
    setlinestyle(1,1,0);
    for(int pos=0; pos<getmaxx(); pos+=20)
    {
        line(0,pos,getmaxx(),pos);
        line(pos,0,pos,getmaxy());
    }

    setlinestyle(0,1,3);
    setcolor(WHITE);
    line(originX,0,originX,getmaxy());
    line(0,originY,getmaxx(),originY);


    char xPlane[60];
    float positiveIncrease=0.0;
    for(int i=0; i<21; i++)
    {
        sprintf(xPlane,"%d",i);
        if(i==0)
        {
            printf(" ");
        }
        else
        {
            outtextxy(originX+positiveIncrease-5,originY+5,xPlane);
            outtextxy(originX+5,originY-positiveIncrease-7,xPlane);
        }
        positiveIncrease+=20.5;
    }

    char yPlane[60];
    float negativeIncrease=0.0;

    for(int i=0; i>-21; i--)
    {
        sprintf(yPlane,"%d",i);
        if(i==0)
        {
            printf(" ");
        }
        else
        {
            outtextxy(originX-negativeIncrease,originY+5,yPlane);
            outtextxy(originX+4,originY+negativeIncrease-7,yPlane);
        }
        negativeIncrease+=20.5;
    }
    setcolor(WHITE);
    char buf[128];
    sprintf(buf, "\Center: = ( x = %.2f , y = %.2f )", inclusiveCircle.x,inclusiveCircle.y);
    outtextxy(originX-410,originY-340, buf);
    sprintf(buf, "Radius: = ( r = %.2f )", inclusiveCircle.r);
    outtextxy(originX-410,originY-320, buf);

    printf("\nCenter: = ( x = %.2f , y = %.2f )\n", inclusiveCircle.x,inclusiveCircle.y);
    printf("Radius: = ( r = %.2f )\n", inclusiveCircle.r);
}
