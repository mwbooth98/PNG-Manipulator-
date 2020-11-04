
#include "stats.h"

stats::stats(PNG & im){
/* Your code here!! */
    int width = im.width();
    int height = im.height();
    sumRed = vector< vector < long > > (width, vector <long> (height));
    sumGreen = vector< vector < long > > (width, vector <long> (height));
    sumBlue = vector< vector < long > > (width, vector <long> (height));
    sumsqRed = vector< vector < long > > (width, vector <long> (height));
    sumsqGreen= vector< vector < long > > (width, vector <long> (height));
    sumsqBlue = vector< vector < long > > (width, vector <long> (height));
    for(int i=0; i<width; i++){
        for (int j=0;j<height;j++){
            long r = (long)im.getPixel(i,j)->r;
            long g = (long)im.getPixel(i,j)->g;
            long b = (long)im.getPixel(i,j)->b;
            if(i==0 && j==0){
                sumRed[0][0]=r;
                sumBlue[0][0]=b;
                sumGreen[0][0]=g;
                sumsqBlue[0][0]=b*b;
                sumsqGreen[0][0]=g*g;
                sumsqRed[0][0]=r*r;
            }else if (i==0){
                sumRed[i][j] = sumRed[i][j-1]+r;
                sumBlue[i][j] = sumBlue[i][j-1] + b;
                sumGreen[i][j] = sumGreen[i][j-1] + g;
                sumsqBlue[i][j] = sumsqBlue[i][j-1] + b*b;
                sumsqRed[i][j] = sumsqRed[i][j-1] + r*r;
                sumsqGreen[i][j] = sumsqGreen[i][j-1] + g*g;
            }else if (j==0){
                sumRed[i][j] = sumRed[i-1][j]+r;
                sumBlue[i][j] = sumBlue[i-1][j] + b;
                sumGreen[i][j] = sumGreen[i-1][j] + g;
                sumsqGreen[i][j] = sumsqGreen[i-1][j] + g*g;
                sumsqBlue[i][j] = sumsqBlue[i-1][j] + b*b;
                sumsqRed[i][j] = sumsqRed[i-1][j] + r*r;
            }else{
                sumRed[i][j] = r + sumRed[i][j-1]+sumRed[i-1][j]-sumRed[i-1][j-1];
                sumGreen[i][j] = g + sumGreen[i][j-1]+sumGreen[i-1][j]-sumGreen[i-1][j-1];
                sumBlue[i][j] = b + sumBlue[i][j-1]+sumBlue[i-1][j]-sumBlue[i-1][j-1];
                sumsqRed[i][j] = r*r + sumsqRed[i][j-1]+sumsqRed[i-1][j]-sumsqRed[i-1][j-1];
                sumsqGreen[i][j] = g*g + sumsqGreen[i][j-1]+sumsqGreen[i-1][j]-sumsqGreen[i-1][j-1];
                sumsqBlue[i][j] = b*b + sumsqBlue[i][j-1]+sumsqBlue[i-1][j]-sumsqBlue[i-1][j-1];            
            }
        }
    }
}


long stats::getSum(char channel, pair<int,int> ul, int dim){
/* Your code here!! */
    int length = pow(2,dim)-1;
    long sum = 0;
    int x1=ul.first;
    int y1=ul.second;
    int x2=ul.first+length;
    int y2=ul.second+length;
    if (channel=='r'){
        if (x1==0 && y1==0){
            sum=sumRed[x2][y2];
        }else if (x1==0){
            sum = sumRed[x2][y2]-sumRed[x2][y1-1];
        }else if (y1==0){
            sum = sumRed[x2][y2]-sumRed[x1-1][y2];
        } else {
            sum = sumRed[x2][y2]-sumRed[x1-1][y2]-sumRed[x2][y1-1]+sumRed[x1-1][y1-1];
        }
    }
    else if (channel == 'g'){
        if (x1==0 && y1==0){
            sum=sumGreen[x2][y2];
        }else if (x1==0){
            sum=sumGreen[x2][y2]-sumGreen[x2][y1-1];
        }else if (y1==0){
            sum = sumGreen[x2][y2]-sumGreen[x1-1][y2];
        } else {
            sum = sumGreen[x2][y2]-sumGreen[x1-1][y2]-sumGreen[x2][y1-1]+sumGreen[x1-1][y1-1];
        }
    }else{
        if (x1==0 && y1==0){
            sum=sumBlue[x2][y2];
        }else if (x1==0){
            sum=sumBlue[x2][y2]-sumBlue[x2][y1-1];
        }else if (y1==0){
            sum = sumBlue[x2][y2]-sumBlue[x1-1][y2];
        } else {
            sum = sumBlue[x2][y2]-sumBlue[x1-1][y2]-sumBlue[x2][y1-1]+sumBlue[x1-1][y1-1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int dim){
/* Your code here!! */
    long sum = 0;
    int length = pow(2, dim)-1;
    int x1= ul.first;
    int y1 = ul.second;
    int x2= ul.first+length;
    int y2 = ul.second+length;
    if (channel =='r'){
        if (x1==0 && y1==0){
            sum = sumsqRed[x2][y2];
        } else if (x1==0){
            sum = sumsqRed[x2][y2]-sumsqRed[x2][y1-1];
        }else if (y1==0){
            sum = sumsqRed[x2][y2]-sumsqRed[x1-1][y2];
        }else{
            sum = sumsqRed[x2][y2]-sumsqRed[x1-1][y2]-sumsqRed[x2][y1-1]+sumsqRed[x1-1][y1-1];
        }
    }else if (channel =='g'){
        if (x1==0 && y1==0){
            sum = sumsqGreen[x2][y2];
        } else if (x1==0){
            sum = sumsqGreen[x2][y2]-sumsqGreen[x2][y1-1];
        }else if (y1==0){
            sum = sumsqGreen[x2][y2]-sumsqGreen[x1-1][y2];
        }else{
            sum = sumsqGreen[x2][y2]-sumsqGreen[x1-1][y2]-sumsqGreen[x2][y1-1]+sumsqGreen[x1-1][y1-1];
        }        
    }else{
        if (x1==0 && y1==0){
            sum = sumsqBlue[x2][y2];
        } else if (x1==0){
            sum = sumsqBlue[x2][y2]-sumsqBlue[x2][y1-1];
        }else if (y1==0){
            sum = sumsqBlue[x2][y2]-sumsqBlue[x1-1][y2];
        }else{
            sum = sumsqBlue[x2][y2]-sumsqBlue[x1-1][y2]-sumsqBlue[x2][y1-1]+sumsqBlue[x1-1][y1-1];
        }        
    }
    return sum; 
}

long stats::rectArea(int dim) {
/* Your code here!! */
    return pow(2, dim)*pow(2,dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int dim){
/* Your code here!! */
    int area = rectArea(dim);
    long r=getSumSq('r',ul,dim)-getSum('r',ul,dim)*getSum('r',ul,dim)/area;
    long g=getSumSq('g',ul,dim)-getSum('g',ul,dim)*getSum('g',ul,dim)/area;
    long b=getSumSq('b',ul,dim)-getSum('b',ul,dim)*getSum('b',ul,dim)/area;
    return r+g+b;
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int dim){
/* Your code here!! */
    long red=getSum('r',ul,dim);
    long green=getSum('g',ul,dim);
    long blue=getSum('b',ul,dim);
    long area = rectArea(dim);
    int redavg = (int)red/area;
    int greenavg = (int)green/area;
    int blueavg = (int)blue/area;
    return RGBAPixel(redavg, greenavg, blueavg);
}
