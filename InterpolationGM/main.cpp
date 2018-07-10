//
//  main.cpp
//  InterpolationGM
//
//  Created by Daniel Voloshin and Nikita Vetugov on 7/6/18.
//  Copyright © 2018 Даниил Волошин. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;


//Function that returns the number of lines of a file
int numLines(ifstream &a){
    int numLines=0;
    string line;
    while (getline(a,line)) {
        numLines++;
    }
    a.clear();                 // clear fail and eof bits
    a.seekg(0, std::ios::beg); // back to the start!
    return numLines;
}


//Function that fills the array from the file
void readf(float array[870][5], ifstream &f){
    float q,w,e,r,t;
    int i=0;
    
    while (f >> q >> w >> e >> r >> t) {
        
        array[i][0]=q; array[i][1]=w; array[i][2]=e; array[i][3]=r; array[i][4]=t;
        i++;
        
    }
    f.clear();                 // clear fail and eof bits
    f.seekg(0, std::ios::beg); // back to the start!
}


//Function that interpolates
float interpol(float a,float b,float c,float za,float zb){
    float zc=za+(c-a)/(b-a)*(zb-za);
    return zc;
}


//Function that eliminates X
void elimX(float array1[870][5], float array2[870][5], float x){

    if (x<0) {
        
        for (int i=0; i<870; i++) {
            for (int j=0; j<5; j++) {
                array2[i][j]=interpol(-4, 0, x, 0, array1[i][j]);
            }
        }
    }
    
    if (x>0) {
        
        for (int i=0; i<870; i++) {
            for (int j=0; j<5; j++) {
                array2[i][j]=interpol(0, 4, x, array1[i][j], 0);
            }
        }
    }
    
    if (x==0) {
    
        for (int i=0; i<870; i++) {
            for (int j=0; j<5; j++) {
                array2[i][j]=array1[i][j];
            }
        }
    }
}


//Function that eliminates Y
void elimY(float array1[870][5], float array2[30][5], float g[29], float y){
    float a=0,b=0,za=0,zb=0;
    int q=0;
    bool isPresent=0;
    for (int i=0; i<30; i++) {
        if (y>g[i-1] && y<g[i]) {
            a=g[i-1];
            b=g[i];
            q=i;
        }
        if (y==g[i]) {
            isPresent=1;
            q=i;
        }
    }
    
    if (isPresent==1) {
        int k=0;
        for (int i=30*(q+1)-29-1; i<30*(q+1); i++) {
            for (int j=0; j<5; j++) {
                array2[k][j]=array1[i][j];
            }
            k++;
        }
    }
    
    if (isPresent==0) {
        int k=0;
        for (int i=30*q-29-1; i<30*q; i++) {
            for (int j=0; j<5; j++) {
                za=array1[i][j];
                zb=array1[i+30][j];
                array2[k][j]=interpol(a, b, y, za, zb);
            }
            k++;
        }
    }
}


//Function that eliminates Mach
void elimMach(float array1[30][5], float array2[5][5], float g[6], float y){
    float a=0,b=0,za=0,zb=0;
    int q=0;
    bool isPresent=0;
    for (int i=0; i<7; i++) {
        if (y>g[i-1] && y<g[i]) {
            a=g[i-1];
            b=g[i];
            q=i;
        }
        if (y==g[i]) {
            isPresent=1;
            q=i;
        }
    }
    
    if (isPresent==1) {
        int k=0;
        for (int i=5*(q+1)-4-1; i<5*(q+1); i++) {
            for (int j=0; j<5; j++) {
                array2[k][j]=array1[i][j];
            }
            k++;
        }
    }
    
    if (isPresent==0) {
        int k=0;
        for (int i=5*q-4-1; i<5*q; i++) {
            for (int j=0; j<5; j++) {
                za=array1[i][j];
                zb=array1[i+5][j];
                array2[k][j]=interpol(a, b, y, za, zb);
            }
            k++;
        }
    }
  
}


//Function that eliminates Beta
void elimBeta(float array1[5][5], float array2[5], float g[5], float y){
    float a=0,b=0,za=0,zb=0;
    int q=0;
    bool isPresent=0;
    for (int i=0; i<6; i++) {
        if (y>g[i-1] && y<g[i]) {
            a=g[i-1];
            b=g[i];
            q=i;
        }
        if (y==g[i]) {
            isPresent=1;
            q=i;
        }
    }
    
    if (isPresent==1) {
        for (int j=0; j<5; j++) {
            array2[j]=array1[q][j];
        }
    }
    
    if (isPresent==0) {
        for (int j=0; j<5; j++) {
            za=array1[q-1][j];
            zb=array1[q][j];
            array2[j]=interpol(a, b, y, za, zb);
        }
    }

}


//Function that eliminates Alpha
void elimAlpha(float array1[5], float &answer, float g[5], float y){
    float a=0,b=0,za=0,zb=0;
    int q=0;
    bool isPresent=0;
    for (int i=0; i<6; i++) {
        if (y>g[i-1] && y<g[i]) {
            a=g[i-1];
            b=g[i];
            q=i;
        }
        if (y==g[i]) {
            isPresent=1;
            q=i;
        }
    }
    
    if (isPresent==1) {
        for (int i=0; i<5; i++) {
            answer=array1[q];
        }
    }
    
    if (isPresent==0) {
        for (int i=0; i<5; i++) {
            za=array1[q-1];
            zb=array1[q];
            answer=interpol(a, b, y, za, zb);
        }
    }
}


//Function that finds the max number in an array
float findMax(float b[], int size){
    float max=b[0];
    for (int i=1; i<size; i++) {
        if (b[i]>max) {
            max=b[i];
        }
    }
    return max;
}

//Function that finds the min number in an array
float findMin(float b[], int size){
    float min=b[0];
    for (int i=1; i<size; i++) {
        if (b[i]<min) {
            min=b[i];
        }
    }
    return min;
}







int main() {
    
    //Deal with files
    ifstream fileObj1;
    fileObj1.open("data.txt");
    
    //Declare the main array and fill it with data from the file
    float dataArray[870][5];
    readf(dataArray, fileObj1);
    
    //Output a file containing the array for Swift
    ofstream fileObj5("swift.txt");
    fileObj5 << "[";
    for (int i=0; i<869; i++) {
        fileObj5 << "[";
        for (int j=0; j<4; j++) {
            fileObj5 << dataArray[i][j] << ",";
        }
        fileObj5 << dataArray[i][4] << "],";
    }
    fileObj5 << "]";                             //BUG!!! The penultimate symbol is a comma (it shouldn't be there)
    
    
    //Declare and fill additional arrays of varibales
    float alpha[5]={-4,0,4,8,12};
    float beta[5]={-4,-2,0,2,4};
    float mach[6]={0.2,0.8,1.3,1.7,2,2.3};
    float y[29]={0.0000,0.0910,0.2180,0.3340,0.4560,0.5720,0.6990,0.8210,0.9350,1.0530,1.1780,1.2950,1.4160,1.5420,1.6560,1.7710,1.9010,2.0120,2.1320,2.2560,2.3730,2.4920,3.0000,4.0000,5.0000,6.0000,7.0000,9.0000,11.0000};
    
    float arrayY[870][5];
    float arrayMach[30][5];
    float arrayBeta[5][5];
    float arrayAlpha[5];
    float answer=-3.14;
    
    float inputAlpha, inputBeta, inputMach, inputY, inputX;

    
    cout << "Введите значение alpha от -4 до 12:" << endl;
    cin >> inputAlpha;
    if (inputAlpha < -4 || inputAlpha > 12) {
        cout << "ERROR" << endl;
        return 1;
    }
    cout << "Введите значение beta от -4 до 4:" << endl;
    cin >> inputBeta;
    if (inputBeta < -4 || inputBeta > 4) {
        cout << "ERROR" << endl;
        return 2;
    }
    cout << "Введите значение mach от 0.2 до 2.3:" << endl;
    cin >> inputMach;
    if (inputMach < 0.2 || inputMach > 2.3) {
        cout << "ERROR" << endl;
        return 3;
    }
    cout << "Введите значение Y от 0 до 11:" << endl;
    cin >> inputY;
    if (inputY < 0 || inputY > 11) {
        cout << "ERROR" << endl;
        return 4;
    }
    cout << "Введите значение X от -4 до 4:" << endl;
    cin >> inputX;
    if (inputX < -4 || inputX > 4) {
        cout << "ERROR" << endl;
        return 5;
    }
    cout << endl << endl;
    
    elimX(dataArray,arrayY, inputX);
    elimY(arrayY, arrayMach, y, inputY);
    elimMach(arrayMach, arrayBeta, mach, inputMach);
    elimBeta(arrayBeta, arrayAlpha, beta, inputBeta);
    elimAlpha(arrayAlpha, answer, alpha, inputAlpha);
    
    //cout << setprecision(4);
    cout << "mz = " << answer << endl;
    
    
    
    
 
    //Plot
    float plotArray[1100];
    //Fill the plotArray
    for (int i=0; i<1100; i++) {
        elimX(dataArray,arrayY, inputX); //0.2->inputX
        elimY(arrayY, arrayMach, y, i*0.01);
        elimMach(arrayMach, arrayBeta, mach, inputMach); //2.2->inputMach
        elimBeta(arrayBeta, arrayAlpha, beta, inputBeta); //-4->inputBeta
        elimAlpha(arrayAlpha, answer, alpha, inputAlpha); //4->inputAlpha
        plotArray[i]=answer;
    }
    
    
    //Create a file for Mathematica
    ofstream fileObj3("mathematica.txt");
    fileObj3 << "{";
    for (int i=0; i<1099; i++) {
        fileObj3 << "{" << i*0.01 << "," << plotArray[i] << "},";
    }
    fileObj3 << "{" << 1100*0.01 << "," << plotArray[1099] << "}";
    fileObj3 << "}";
    
    
    //Create a file for yotx.ru
    ofstream fileObj4("yotx.txt");
    for (int i=0; i<1100; i++) {
        fileObj4 << "(" << i*0.01 << ";" << plotArray[i] << ") ";
    }
    
    
    //Create an image
    //Create an empty canvas
    const int width=1100, height=ceil(100*(findMax(plotArray, 1100)-findMin(plotArray, 1100)));
    //cout << "width=" << width << ", height=" << height << endl;
    ofstream fileObj2("plot.ppm");
    fileObj2 << "P3" << endl;
    fileObj2 << width << " " << height << endl;
    fileObj2 << "255" << endl;
    int r,g,b;
    
    //Fill the image
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (height-y==ceil(-100*findMin(plotArray, 1100)+100*plotArray[x])) {
                r=61;
                g=64;
                b=148;
            } else {
                r=255;
                g=255;
                b=255;
            }
            if (height-y==ceil(-100*findMin(plotArray, 1100))) {    //Draw the x-axis
                r=0;
                g=0;
                b=0;
            }
            fileObj2 << r << " " << g << " " << b << endl;
        }
    }
    
    
    
    
    cout << endl << endl << endl;
    
    return 0;
}
