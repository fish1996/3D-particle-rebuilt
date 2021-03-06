#include "detection.h"
#include<QDebug>

detection::detection()//(string tempfilename1, double blocks, double gray1, double gray2)
{
}

detection::~detection()
{
}

//需要全局变量img_f从第二步中得到
//handles.adp_blocksize,handles.global_gray1,handles.global_gray2从界面中得到

int detection::find( int set[], int x )
{
    int r = x;
    while ( set[r] != r )
        r = set[r];
    return r;
}

//得到图像矩阵的连通区域
int detection::bwlabel(Mat &img, Mat &L, int n, int* labels)
{
    if(n != 4 && n != 8)
        n = 4;
    int nr = img.rows;
    int nc = img.cols;

    int total = nr * nc;
    // results
    labels=new int[total];
    memset(labels, 0, total * sizeof(int));
    int nobj = 0;                               // number of objects found in image
    // other variables
    int* lset = new int[total];   // label table
    memset(lset, 0, total * sizeof(int));
    int ntable = 0;

    for( int r = 0; r < nr; r++ )
    {
        for( int c = 0; c < nc; c++ )
        {
            if ( img.at<double>(r,c) )   // if A is an object
            {
                // get the neighboring pixels B, C, D, and E
                int B, C, D, E;
                if ( c == 0 )
                    B = 0;
                else
                    B = find( lset, ONETWO(labels, r, c - 1, nc) );
                if ( r == 0 )
                    C = 0;
                else
                    C = find( lset, ONETWO(labels, r - 1, c, nc) );
                if ( r == 0 || c == 0 )
                    D = 0;
                else
                    D = find( lset, ONETWO(labels, r - 1, c - 1, nc) );
                if ( r == 0 || c == nc - 1 )
                    E = 0;
                else
                    E = find( lset, ONETWO(labels, r - 1, c + 1, nc) );
                if ( n == 4 )
                {
                    // apply 4 connectedness
                    if ( B && C )
                    {        // B and C are labeled
                        if ( B == C )
                            ONETWO(labels, r, c, nc) = B;
                        else {
                            lset[C] = B;
                            ONETWO(labels, r, c, nc) = B;
                        }
                    }
                    else if ( B )             // B is object but C is not
                        ONETWO(labels, r, c, nc) = B;
                    else if ( C )               // C is object but B is not
                        ONETWO(labels, r, c, nc) = C;
                    else
                    {                      // B, C, D not object - new object
                        //   label and put into table
                        ntable++;
                        ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
                    }
                }
                else if ( n == 6 )
                {
                    // apply 6 connected ness
                    if ( D )                    // D object, copy label and move on
                        ONETWO(labels, r, c, nc) = D;
                    else if ( B && C )
                    {        // B and C are labeled
                        if ( B == C )
                            ONETWO(labels, r, c, nc) = B;
                        else
                        {
                            int tlabel = MIN(B,C);
                            lset[B] = tlabel;
                            lset[C] = tlabel;
                            ONETWO(labels, r, c, nc) = tlabel;
                        }
                    }
                    else if ( B )             // B is object but C is not
                        ONETWO(labels, r, c, nc) = B;
                    else if ( C )               // C is object but B is not
                        ONETWO(labels, r, c, nc) = C;
                    else
                    {                      // B, C, D not object - new object
                        //   label and put into table
                        ntable++;
                        ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
                    }
                }
                else if ( n == 8 )
                {
                    // apply 8 connectedness
                    if ( B || C || D || E )
                    {
                        int tlabel = B;
                        if ( B )
                            tlabel = B;
                        else if ( C )
                            tlabel = C;
                        else if ( D )
                            tlabel = D;
                        else if ( E )
                            tlabel = E;
                        ONETWO(labels, r, c, nc) = tlabel;
                        if ( B && B != tlabel )
                            lset[B] = tlabel;
                        if ( C && C != tlabel )
                            lset[C] = tlabel;
                        if ( D && D != tlabel )
                            lset[D] = tlabel;
                        if ( E && E != tlabel )
                            lset[E] = tlabel;
                    }
                    else
                    {
                        //   label and put into table
                        ntable++;
                        ONETWO(labels, r, c, nc) = lset[ ntable ] = ntable;
                    }
                }
            }
            else
            {
                ONETWO(labels, r, c, nc) = NO_OBJECT;      // A is not an object so leave it
            }
        }
    }

    // consolidate component table
    for( int i = 0; i <= ntable; i++ )
        lset[i] = find( lset, i );
    // run image through the look-up table
    for( int r = 0; r < nr; r++ )
        for( int c = 0; c < nc; c++ )
            ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];
    // count up the objects in the image
    for( int i = 0; i <= ntable; i++ )
        lset[i] = 0;
    for( int r = 0; r < nr; r++ )
        for( int c = 0; c < nc; c++ )
            lset[ ONETWO(labels, r, c, nc) ]++;
    // number the objects from 1 through n objects
    nobj = 0;
    lset[0] = 0;
    for( int i = 1; i <= ntable; i++ ){
        if ( lset[i] > 0 )
            lset[i] = ++nobj;
    }
    // run through the look-up table again
    for( int r = 0; r < nr; r++ )
        for( int c = 0; c < nc; c++ )
            ONETWO(labels, r, c, nc) = lset[ ONETWO(labels, r, c, nc) ];
    //
    delete[] lset;

    for(int i=0;i<L.rows;i++){
        for(int j=0;j<L.cols;j++){
            L.at<double>(i,j)=labels[i*(L.cols)+j];
        }
    }

    return nobj;
}

//二值化，超过thresh，取maximal，反之为0
void detection::dothreshold(Mat &inputimg,Mat &outputimg,double thresh, double maxval)
{
    outputimg=inputimg.clone();
    for(int i=0;i<inputimg.rows;i++){
        for(int j=0;j<inputimg.cols;j++){
            if(inputimg.at<double>(i,j)>thresh){
                outputimg.at<double>(i,j)=maxval;
            }
            else{
                outputimg.at<double>(i,j)=0;
            }
        }
    }
}

//取矩阵极值
void detection::extreme(Mat &imageout, double &maxv, double &minv)
{
    maxv=minv=0.0; //初始化
    minMaxIdx(imageout,&minv,&maxv);
}

//去除图像亮度孤立点，即将被0包围的1均变为0
void detection::bwmorph(Mat &inputimg,Mat &outputimg)
{
    Size size=inputimg.size();
    int Iy=size.height;
    int Ix=size.width;
    Mat extendimg(Iy+2, Ix+2, CV_64F, Scalar(0));
    for(int i=0;i<inputimg.rows;i++){
        for(int j=0;j<inputimg.cols;j++){
            extendimg.at<double>(i+1,j+1) = inputimg.at<double>(i,j);
        }
    }
    for(int i=1;i<extendimg.rows-1;i++){
        for(int j=1;j<extendimg.cols-1;j++){
            if(extendimg.at<double>(i,j)==1){
                if((extendimg.at<double>(i-1,j-1)==0)&&(extendimg.at<double>(i-1,j)==0)&&(extendimg.at<double>(i-1,j+1)==0)
                   &&(extendimg.at<double>(i,j-1)==0)&&(extendimg.at<double>(i,j+1)==0)
                   &&(extendimg.at<double>(i+1,j-1)==0)&&(extendimg.at<double>(i+1,j)==0)&&(extendimg.at<double>(i+1,j+1)==0))
                {
                    extendimg.at<double>(i,j)=0;
                }

            }
        }
    }
    outputimg=extendimg(Range(1,Iy+1),Range(1,Ix+1));
}

//填空函数imfill，把0填为1
void detection::fillHole(const Mat srcBw, Mat &dstBw)
{
    //拓展边，均为0
    Size m_Size = srcBw.size();
    Mat Temp=Mat::zeros(m_Size.height+2,m_Size.width+2,srcBw.type());//延展图像
    srcBw.copyTo(Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));

    //floodFill(Temp, Point(0, 0), Scalar(1));
    //setp 2: find the contour fill holes填洞
    vector<Vec4i> hierarchy;
    typedef vector<vector<Point> > TContours;
    TContours contours;
    findContours(Temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    //
    // fill external contours
    //
    if( !contours.empty() && !hierarchy.empty() )
    {
        for (int idx=0;idx < contours.size();idx++)
        {
            drawContours(Temp,contours,idx,Scalar::all(1),CV_FILLED,8);
        }
    }

    Mat cutImg;//裁剪延展的图像
    Temp(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);

    cutImg.convertTo(dstBw, CV_64F);
}

mxArray* detection::mat2mxarray(Mat imgsrc)
{
    mxArray * pv;
    if (!imgsrc.empty())
    {
      imgsrc = imgsrc.t();
      pv = mxCreateNumericMatrix(imgsrc.cols,imgsrc.rows,mxDOUBLE_CLASS, mxREAL);
      memcpy(mxGetPr(pv), imgsrc.data, mxGetNumberOfElements(pv)*sizeof(double));
    }
    else
    {
      printf("GaborFeais empty! \n");
      return false;
    }
    return pv;
}

Mat detection::mxarray2mat(mxArray *data)
{
    int m=mxGetM(data);
    int n=mxGetN(data);
    int channels;
    int cvmtype;
    mwSize ndim=mxGetNumberOfDimensions(data);
    if(ndim==3){
        channels=3;
        cvmtype=CV_64FC3;
    }
    else if(ndim==2){
        channels=1;
        cvmtype=CV_64FC1;
    }
    else
        cout<<"Input matrix should be a 2d or 3d matrix."<<endl;
    int rows=m;
    int cols=n/channels;

    double* inimgmat=(double*)mxGetData(data);
    Mat in=Mat(rows,cols,cvmtype);
    if(channels==1)
    {
        double* ucp;
        for(int i=0;i<rows;i++){
            ucp=in.ptr<double>(i);
            for(int j=0;j<cols*channels;j++){
                ucp[j]=(double)*(inimgmat+i+j*rows);
            }
        }
    }
    else if(channels==3){
        Vec3b* ucp;
        for(int i=0;i<rows;i++){
            ucp=in.ptr<Vec3b>(i);
            for(int j=0;j<cols;j++){
                ucp[j][2]=(uchar)*(inimgmat+i+j*rows);
                ucp[j][1]=(uchar)*(inimgmat+i+j*rows+rows*cols*1);
                ucp[j][0]=(uchar)*(inimgmat+i+j*rows+rows*cols*2);
            }
        }
    }
    return in;
}

void detection::adaptivethreshold(string tempfilename,double blocksize,double globalgraythreshhold,double globalgraythreshold2,int imgnum)
{
    p1xy=new Mat[imgnum]();
    p1area=new Mat[imgnum]();
    p1box=new Mat[imgnum]();

    Engine *ep;
    if (!(ep=engOpen(NULL))) //测试是否启动Matlab引擎成功。
    {
        qDebug() <<"Can't start Matlab engine!" <<endl;
        exit(1);
    }
    qDebug()<<"Open Matlab Engine!" <<endl;

    for(int imgn=0;imgn<imgnum;imgn++){
        qDebug()<<"imgn="<<imgn;
        char img_fname[1000];
        sprintf(img_fname,"%s%s%d%s",tempfilename.c_str(),"\\img_fuse",imgn,".bmp");//需要根据实际改
        Mat imgtotal;
        Mat imgcolor=imread(img_fname,CV_LOAD_IMAGE_GRAYSCALE);
        imgcolor.convertTo(imgtotal,CV_64F,1.0/255.0);
        imgcolor.release();

        Mat imgbw;
        Mat L1;

        Size size=imgtotal.size();
        int ny=size.height;
        int nx=size.width;

        /*Scalar mean;
        Scalar stddev;
        meanStdDev ( imgtotal, mean, stddev );
        double imean = mean.val[0];*/

        Mat imbw1;
        imgbw=Mat::zeros(ny, nx, CV_64F);

        for(int i=1;i<=ny;i=i+blocksize){
            for(int j=1;j<=nx;j=j+blocksize){
                int jmax=j+blocksize-1<nx?j+blocksize-1:nx;
                int imax=i+blocksize-1<ny?i+blocksize-1:ny;

                Mat img=imgtotal(Range(i-1,imax),Range(j-1,jmax));

                double max;
                double min;
                extreme(img,max,min);

                Scalar mean1;
                Scalar stddev1;
                meanStdDev ( img, mean1, stddev1 );
                double imean1 = mean1.val[0];

                double level1=(max+imean1)/2;

                if(globalgraythreshold2==0.0f){
                    double tempthreshold=(imean1*1.2)>globalgraythreshhold?imean1*1.2:globalgraythreshhold;
                    dothreshold(img,imbw1,tempthreshold,1);
                }
                else if(globalgraythreshold2==1.0f){
                    dothreshold(img,imbw1,globalgraythreshhold,1);
                }
                else{
                    double tempthreshold1=(imean1*1.2)>globalgraythreshhold?imean1*1.2:globalgraythreshhold;
                    double tempthreshold2=tempthreshold1>level1?tempthreshold1:level1;
                    dothreshold(img,imbw1,tempthreshold2,1);
                }

                bwmorph(imbw1,imbw1);

                int *labels;
                L1=Mat::zeros(imbw1.rows, imbw1.cols, CV_64F);
                int num1;
                num1=bwlabel(imbw1, L1, 8, labels);

                L1.copyTo(imgbw(Range(i-1, imax), Range(j-1, jmax)));

            }
            emit(detectOk());
        }
        Mat bw;
        imgbw.convertTo(bw, CV_8U);
        Mat_<double> bwFill;
        fillHole(bw, bwFill);
        imgbw=bwFill.clone();

        int *ilabels;
        L1=Mat::zeros(imgbw.rows, imgbw.cols, CV_64F);
        int inum1;
        inum1=bwlabel(imgbw, L1, 8, ilabels);

        normalize(L1,L1,0,255,NORM_MINMAX);
        char img_lname[1000];
        sprintf(img_lname,"%s%s%d%s",tempfilename.c_str(),"\\img_binaryzation",imgn,".jpg");//需要根据实际改
        imwrite(img_lname,L1);

        mxArray *pimgbw=NULL;
        pimgbw=mat2mxarray(imgbw);

        engPutVariable(ep,"pimgbw",pimgbw);
        engEvalString(ep,"[pL1, num1] = bwlabel(pimgbw, 8);"); //保存数据于matlab形式
        engEvalString(ep,"seg = regionprops(pL1, 'Area','centroid','BoundingBox')");    //调用matlab的 regionprops库函数

        engEvalString(ep,"p1xy=cat(1,seg.Centroid);p1area=cat(1,seg.Area);p1box=cat(1,seg.BoundingBox);");
        mxArray* p2xy=engGetVariable(ep,"p1xy");
        mxArray* p2area=engGetVariable(ep,"p1area");
        mxArray* p2box=engGetVariable(ep,"p1box");

        p1xy[imgn]=mxarray2mat(p2xy);
        p1area[imgn]=mxarray2mat(p2area);
        p1box[imgn]=mxarray2mat(p2box);

        mxDestroyArray(p2xy);
        mxDestroyArray(p2area);
        mxDestroyArray(p2box);
        mxDestroyArray(pimgbw);

        bwFill.release();
        bw.release();
        imbw1.release();
    }

    if (!engClose(ep)) //测试是否关闭Matlab引擎成功。
    {
        qDebug()<<"Close Matlab Engine!" <<endl;
        //exit(1);
    }else  qDebug()<<"Can't close Matlab engine!" <<endl;

}
