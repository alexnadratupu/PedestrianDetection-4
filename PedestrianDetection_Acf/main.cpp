//
//  main.cpp
//  PedestrianDetection_Acf
//
//  Created by 董昭 on 17/8/28.
//  Copyright © 2017年 董昭. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include<vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml.hpp>

#include "PedDetector.h"
using namespace std;
using namespace cv;

#define DataSet "/Users/Dzhaoooo/Desktop/CarCameraData4/2015-09-23-09-44-41"  //测试数据集路径（正例）
// ChnFtrsAdaboost.cpp : 定义控制台应用程序的入口点。FPDW版
//获取指定文件夹下的所有文件名列表
vector<string> getAllFiles(){
    DIR *dp;
    struct dirent *dirp;
    vector<string> filenames;
    
    dp=opendir(DataSet);
    while((dirp=readdir(dp))!=NULL){
        if(strcmp(dirp->d_name, ".")!=0 && strcmp(dirp->d_name, "..")!=0 &&strcmp(dirp->d_name, ".DS_Store")!=0){
            filenames.push_back(dirp->d_name);
        }
    }
    return filenames;
    
}

void acfDetect(string filename,int index){
    PedDetector pd;
    pd.loadStrongClassifier("/Users/Dzhaoooo/Desktop/PedestrianDetection_Acf/PedestrianDetection_Acf/ClassifierOut.txt");
    string ImgName="/Users/Dzhaoooo/Desktop/CarCameraData4/2015-09-23-09-44-41/"+filename;
    cout<<"正在处理图片："<<ImgName<<endl;
    const char *srcImg=ImgName.data();
    IplImage *img = cvLoadImage(srcImg);
    CvMat *ReMat=NULL;
    pd.Detection_FPDW(img, &ReMat, 3);
    pd.show_detections(img, ReMat);
    
    string saveName;
    stringstream temp;
    string indexStr;
    temp<<index;
    temp>>indexStr;
    saveName="/Users/Dzhaoooo/Desktop/CarCameraData4/ResultData/"+indexStr+".jpg";
    const char * result=saveName.data();
    cvSaveImage(result, img);
    
}
int main(int argc, const char * argv[])
{
    vector<string> filenames=getAllFiles();
    size_t size=filenames.size();
    for(int i=0;i<size;i++)
    {
//        ifstream infile("/Users/Dzhaoooo/Desktop/RectInform.json");
        ofstream outfile;
        String filname = (String)filenames[i];
        
        outfile.open("/Users/Dzhaoooo/Desktop/CarCameraData4/JsonInform/"+filname+".json");
        acfDetect(filname, i);
        ifstream infile("/Users/Dzhaoooo/Desktop/RectInform.json");
        string line;
        outfile<<"\"filename\":"<<filname<<"\",\"objects\":[";
        int temp = 0;
//        while(getline(infile, line)){
//            cout<<line<<endl;
////            outfile<<"\"filename\":"<<filname<<"\",\"objects\":[";
//            outfile<<line<<",";
//        }
        while(getline(infile, line)){
            if(temp>0)
                outfile<<",";
//            outfile<<"\"filename\":"<<filname<<"\",\"objects\":["<<line;
            outfile<<line;
            temp++;
            
        }
        outfile<<"]}"<<endl;
        outfile.close();
        infile.close();
    }
    
    return 0;
}

