#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
using namespace std;
void ppmRead(char* filename, unsigned char* data, int* w, int* h) {
	char header[1024];
	FILE* fp = NULL;
	int line = 0;

	fp = fopen(filename, "rb");
	while (line < 2) {
		fgets(header, 1024, fp);
		if (header[0] != '#') {
			++line;
		}
	}
	sscanf(header, "%d %d\n", w, h);
	fgets(header, 20, fp);
	fread(data, (*w)*(*h) * 3, 1, fp);

	fclose(fp);
}
void ppmWrite(const char* filename, unsigned char* data, int w, int h) {
	FILE* fp;
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	fwrite(data, w*h * 3, 1, fp);

	fclose(fp);
}

void paintEllipse(int centerX, int centerY, int Ra, int Rb, unsigned char* data, int w, int h,double rotate){
    //draw background
    for(int i = 0;i<w;i++){
        for(int j = 0;j< h;j++){
            data[(i + j*w) * 3 + 0] = 255;
            data[(i + j*w) * 3 + 1] = 255;
            data[(i + j*w) * 3 + 2] = 255;
        }
    }
    // draw axis
    for(int i = 0;i<w;i++){
        data[(i+h*w/2)*3+0] = 0;
        data[(i+h*w/2)*3+1] = 0;
        data[(i+h*w/2)*3+2] = 0;
    }
    for(int i = 0;i<h;i++){
        data[(w/2+i*w)*3+0] = 0;
        data[(w/2+i*w)*3+1] = 0;
        data[(w/2+i*w)*3+2] = 0;
    }
    // draw circle
    for(int i = centerX-4;i<centerX+4;i++){
        for(int j = centerY-4;j<centerY+4;j++){
            if((i-centerX)*(i-centerX)+(j-centerY)*(j-centerY) <= 16){
                if(i<0 || i>=w || j<0 || j>=h) {
                    continue;
                }
                data[(i+j*w)*3+0] = 255;
                data[(i+j*w)*3+1] = data[(i+j*w)*3+2] = 0;
            }
        }
    }
    int x ,y;
    for(double theta = 0;theta<2*M_PI;theta+=0.001){
        x = (int)(Ra*cos(theta));
        y = (int)(Rb*sin(theta));
        int RotatedX = (int)(cos(rotate)*x + sin(rotate)*y);
        int RotatedY = (int)(-sin(rotate)*x + cos(rotate)*y);
        x = RotatedX+centerX;
        y = RotatedY+centerY;
        if(x<0 || x>=w || y<0 || y>=h) {
            continue;
        }
        data[(x + y*w) * 3 + 0] = data[(x + y*w) * 3 + 2] = 0;
        data[(x + y*w) * 3 + 1] = 255;
    }
}
int main() {
	int x,y,Ra,Rb;
    double theta = 60*M_PI/180;
	x = 250;
	y = 100;
	Ra = 100;
	Rb = 50;
/*
   TODO:
   cout<<"Please input the center of the ellipse (x,y) [from (0,0) to (400,300)]:"<<endl;
   cin>>x>>y;
   cout<<"Please input the radius of the ellipse (Ra,Rb) [from (0,0) to (400,300)]:"<<endl;
   cin>>Ra>>Rb;
   cout<<"Please input the angle of the ellipse (theta) [from (0,0) to (400,300)]:"<<endl;
   cin>>theta;
   theta = theta*M_PI/180;
*/
	unsigned char data[400*300*3] = { 0 };
	paintEllipse(x, y, Ra, Rb, data, 400, 300, theta);
	ppmWrite("test.ppm", data, 400, 300);
	return 0;
}