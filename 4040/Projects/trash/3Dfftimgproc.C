
#include "3Dfftimgproc.h"
#include "LWE.h"
#include <iostream>
using namespace img;

FFT3DImgProc::FFT3DImgProc() :
   Nx(0),
   Ny(0),
   Nz(0),
   img_data(nullptr)
{}

FFT3DImgProc::~FFT3DImgProc(){ clear(); }

void FFT3DImgProc::clear()
{
   if( img_data != nullptr )
   {
      for(size_t i=0;i<forward.size();i++){ fftw_destroy_plan(forward[i]); }
      for(size_t i=0;i<backward.size();i++){ fftw_destroy_plan(backward[i]); }
      fftw_free(img_data);
      img_data = nullptr;
   }
   Nx = 0;
   Ny = 0;
   Nz = 0;
   Nsize = 0;
}

void FFT3DImgProc::clear(int nX, int nY, int nZ)
{
   clear();
   Nx = nX;
   Ny = nY;
   Nz = nZ;
   Nsize = Nx*Ny*Nz;
   img_data = (fftw_complex*)fftw_malloc( sizeof(fftw_complex)*Nx*Ny*Nz );
#pragma omp parallel for
   for(long i=0;i<Nsize;i++){ img_data[i][0] = 0.0;  img_data[i][1] = 0.0; }

    forward = fftw_plan_dft_2d( Ny, Nx, Nz, img_data, img_data, FFTW_FORWARD, FFTW_ESTIMATE);
    backward = fftw_plan_dft_2d( Ny, Nx, Nz, img_data, img_data, FFTW_BACKWARD, FFTW_ESTIMATE);
}

std::complex<double> FFT3DImgProc::value(int i, int j, int k) const
{
   std::complex<double> v(0.0, 0.0);
   if(img_data == nullptr){return;}
   if(i < 0 || i >= Nx){return;}
   if(j < 0 || j >= Ny){return;}
   if(k < 0 || k >= Nz){return;}
   const fftw_complex& vf = img_data[index(i, j, k)];
   v = std::complex<double>(vf[0], vf[1]);
   return v;
}



FFT3DImgProc::FFT3DImgProc(const FFT3DImgProc& v) :
  Nx (v.Nx),
  Ny (v.Ny),
  Nz (v.Nz),
  Nsize (v.Nsize)
{
   img_data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Nx*Ny*Nz);
#pragma omp parallel for
   for(long i = 0; i < Nsize; i++){img_data[i][0] = v.img_data[i][0];  img_data[i][1] = v.img_data[i][1];}

   forward = fftw_plan_dft_2d(Ny, Nx, Nz, img_data, img_data, FFTW_FORWARD, FFTW_ESTIMATE);
   backward = fftw_plan_dft_2d(Ny, Nx, Nz, img_data, img_data, FFTW_BACKWARD, FFTW_ESTIMATE);
}

FFT3DImgProc& FFT3DImgProc::operator=(const FFT3DImgProc& v)
{
   if(this == &v){return *this;}
   if(Nx != v.Nx || Ny != v.Ny || Nz != v.Nz)
   {
      clear();
      Nx = v.Nx;
      Ny = v.Ny;
      Nz = v.Nz;
      Nsize = v.Nsize;
   }

   img_data = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)*Nx*Ny*Nz);
#pragma omp parallel for
   for(long i = 0; i < Nsize; i++){img_data[i][0] = v.img_data[i][0]; img_data[i][1] = v.img_data[i][1];}

   forward = fftw_plan_dft_2d(Ny, Nx, Nz, img_data, img_data, FFTW_FORWARD, FFTW_ESTIMATE);
   backward = fftw_plan_dft_2d(Ny, Nx, Nz, img_data, img_data, FFTW_BACKWARD, FFTW_ESTIMATE);
   return *this;
}


void FFT3DImgProc::set_value(int i, int j, const std::vector<std::complex<double> >& pixel)
{
   if(img_data == nullptr ){return;}
   if(i < 0 || i >= Nx){return;}
   if(j < 0 || j >Ny){return;}
   if(pixel.size() != (size_t)Nz){return;}
   for(int c = 0; c < Nz; c++ )
   {
      fftw_complex& v = img_data[index(i, j, c)];
      v[0] = pixel[c].real();
      v[1] = pixel[c].imag();
   }
   return;
}



void FFT3DImgProc::fft_forward()
{
   for(size_t i = 0; i < forward.size(); i++){fftw_execute(forward[i]);}
}

void FFT3DImgProc::fft_backward()
{
   for(size_t i = 0; i < backward.size(); i++){fftw_execute(backward[i]);}
   double norm = 1.0/(Nx * Ny);
   for(int j = 0; j < Ny; j++)
   {
#pragma omp parallel for
      for(int i = 0; i < Nx; i++)
      {
         for(int c = 0; c < Nz; c++)
	 {
	    img_data[index(i, j, c)][0] *= norm;
	    img_data[index(i, j, c)][1] *= norm;
	 }
      }
   }
}

double FFT3DImgProc::kx(int i) const
{
   double v = (double)i/Nx;
   if(i > Nx/2){v -= 1.0;}
   return 2.0*3.14159265*v;
}

double FFT3DImgProc::ky(int j) const
{
   double v = (double)j/Ny;
   if(j > Ny/2){v -= 1.0;}
   return 2.0*3.14159265*v;
}


long FFT3DImgProc::index(int i, int j, int c) const
{
   // return (long) c + (long) Nz * index(i,j); // interleaved channels

   return index(i,j) + (long)Nx * (long)Ny * (long)c; // sequential channels
}

long FFT3DImgProc::index(int i, int j) const
{
   return (long) i + (long)Nx * (long)j;
}

////EXTRA FUNCTIONS/////////////////////////////////////////////////////////////////////////////////
//START ADDED CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LinearWaveEstimate::doLinearWaveEstimate(){
  LinearWaveEstimate::LinearWaveEstimate(const ImgProc& init, const double dispersion_factor);
}

void ImgProc::doCoherentEstimation(){
  int channel0 = 0;
  int channel1 = 1;
  float weight0 = 1.0;
  float weight1 = 0.5176;
  ImgProc out;
  coherentEstimation(*this, channel0, channel1, weight0, weight1, out);
  swap(*this, out);
}

void ImgProc::coherentEstimation(const ImgProc& image, int channel0, int channel1, float weight0, float weight1, ImgProc& output){
  double avg0 = 0, avg1 = 0, sigma00 = 0, sigma01 = 0, sigma11 = 0;

  for(int i = 0; i < image.ny(); i++){
    for(int j = 0; j < image.nx(); j++){
      std::vector<float> v;
      image.value(j, i, v);
      avg0 += v[channel0];
      avg1 += v[channel1];
    }
  }

  avg0 /= image.nx() * image.ny();
  avg1 /= image.nx() * image.ny();

  for(int i = 0; i < image.ny(); i++){
    for(int j = 0; j < image.nx(); j++){
      std::vector<float> v;
      image.value(j, i, v);
      sigma00 += (v[channel0] - avg0) * (v[channel0] - avg0);
      sigma01 += (v[channel0] - avg0) * (v[channel1] - avg1);
      sigma11 += (v[channel1] - avg1) * (v[channel1] - avg1);
    }
  }

  sigma00 /= image.nx() * image.ny();
  sigma01 /= image.nx() * image.ny();
  sigma11 /= image.nx() * image.ny();

  float determinant = (sigma00 * sigma11) - (sigma01 * sigma01);
  float denominator = (weight0 * weight0 * sigma11) - (2.0 * weight0 * weight1 * sigma01) + (weight1 * weight1 * sigma11);
  denominator = denominator/determinant;

  output = image;

  for(int i = 0; i < image.ny(); i++){
#pragma omp parallel for
    for(int j = 0; j < image.nx(); j++){
      std::vector<float> v;
      image.value(j, i, v);
      float coherent_estimation = (weight0 * (v[channel0] - avg0) * sigma11) - (weight0 * (v[channel1] - avg1) * sigma01) - (weight1 * (v[channel0] - avg0) * sigma01) * (weight1 * (v[channel1] - avg1) * sigma00);
      for(size_t a = 0; a < v.size(); a++){
        v[a] = coherent_estimation;
      }
      output.set_value(j, i, v);
    }
  }
}

struct ImgProc::Point{
  double x = 0.0;
  double y = 0.0;
};

void ColorLUT::operator()(const double& value, std::vector<float>& C) const{
  C = black;
  if(value > 1.0 || value < 0.0){
    return;
  }

  double x = std::pow(value,gamma)*(bands.size()-1);
  size_t low_index = (size_t)x;
  size_t high_index = low_index + 1;

  double weight = x - (double)low_index;

  if(high_index >= bands.size()){high_index = bands.size()-1;}

  for(size_t c = 0; c < C.size(); c++){
    C[c] = bands[low_index][c] * (1.0-weight) + bands[high_index][c] * weight;
  }
}

ColorLUT::ColorLUT(double gam) : gamma (gam){
  std::vector<float> C;
  C.push_back(0.0);
  C.push_back(0.0);
  C.push_back(0.0);
  black = C;
  // Clemson Orange
  C[0] = 245.0/255.0;
  C[1] = 102.0/255.0;
  C[2] = 0.0/255.0;
  bands.push_back(C);
  // Goal Line
  C[0] = 255.0/255.0;
  C[1] = 255.0/255.0;
  C[2] = 255.0/255.0;
  bands.push_back(C);
  // Regalia
  C[0] = 82.0/255.0;
  C[1] = 45.0/255.0;
  C[2] = 128.0/255.0;
  bands.push_back(C);
  // College Avenue
  C[0] = 51.0/255.0;
  C[1] = 51.0/255.0;
  C[2] = 51.0/255.0;
  bands.push_back(C);
  // Diploma
  C[0] = 46.0/255.0;
  C[1] = 26.0/255.0;
  C[2] = 71.0/255.0;
  bands.push_back(C);
  // Bowman Field
  C[0] = 84.0/255.0;
  C[1] = 98.0/255.0;
  C[2] = 35.0/255.0;
  bands.push_back(C);
  // Blue Ridge
  C[0] = 0.0/255.0;
  C[1] = 94.0/255.0;
  C[2] = 184.0/255.0;
  bands.push_back(C);
  // State Flag
  C[0] = 0.0/255.0;
  C[1] = 32.0/255.0;
  C[2] = 91.0/255.0;
  bands.push_back(C);
  // Howard's Rock
  C[0] = 140.0/255.0;
  C[1] = 130.0/255.0;
  C[2] = 121.0/255.0;
  bands.push_back(C);
}

void ImgProc::doIFS(ImgProc& image){
  image.clear(image.nx(), image.ny(), 4);
  size_t nb_iter = 500000;
  ColorLUT lut;
  FractalFlameIFS(nb_iter, lut, image);
  glutPostRedisplay();
}

void ImgProc::FractalFlameIFS(const size_t iterations, const ColorLUT& lut, ImgProc& out){
  Point P;
  srand48(time(0));
  P.x = 2.0*drand48() - 1.0;
  P.y = 2.0*drand48() - 1.0;
  float w = drand48();

  for(size_t iter = 0; iter < iterations; iter++){
    int i = (int)(4*drand48()) % 4;
    float n = 0.0;
    if(i==0){
      n = fractalPolar(P);
      w = ((w + n) * 0.5);
    }else if(i==1){
      n = fractalHandkerchief(P);
      w = ((w + n) * 0.5);
    }else if(i==2){
      n = fractalHeart(P);
      w = ((w + n) * 0.5);
    }else if(i==3){
      n = fractalDisc(P);
      w = ((w + n) * 0.5);
    }

    if(iter > 20){
      if(P.x >= -1.0 && P.x <= 1.0 && P.y >= -1.0 && P.y <= 1.0){
        float x = P.x + 1.0;
        float y = P.y + 1.0;
        x *= 0.5*out.nx();
        y *= 0.5*out.ny();
        int j = x; //j and k could be equal to x and y
        if(j < out.nx()){
          int k = y;
          if(k < out.ny()){
            std::vector<float> color;
            lut(w, color);

            std::vector<float> Cp;
            out.value(j, k, Cp);
            for(size_t m=0; m<Cp.size()-1; m++){
              Cp[m] = Cp[m]*Cp[Cp.size()-1];
              Cp[m] = (Cp[m] + color[m]) / (Cp[Cp.size()-1] + 1);
            }
            Cp[Cp.size()-1] += 1;
            out.set_value(j, k, Cp);
          }
        }
      }
    }
  }
}

float ImgProc::fractalPolar(Point& P){
  //(x,y) = (0/π, r −1);
  float x = P.x, y = P.y;
  float theta = atan2(x,y);
  float r = sqrt((x*x)+(y*y));
  float pi = 3.14159265;
  P.x = theta/pi;
  P.y = r - 1;
  return 0.2;
}

float ImgProc::fractalHandkerchief(Point& P){
  //(x,y) = r · (sin(theta + r), cos(theta − r))
  float x = P.x, y = P.y;
  float theta = atan2(x,y);
  float r = sqrt((x*x)+(y*y));
  P.x = r * (sin(theta + r));
  P.y = r * (cos(theta - r));
  return 0.4;
}

float ImgProc::fractalHeart(Point& P){
  //(x,y) = r · (sin(theta * r), − cos(theta * r))
  float x = P.x, y = P.y;
  //std::cout << "x = " << x << " and y = " << y << std::endl;
  float theta = atan2(x,y);
  float r = sqrt((x*x)+(y*y));
  P.x = r * (sin(theta * r));
  P.y = r * (-cos(theta * r));
  return 0.6;
}

float ImgProc::fractalDisc(Point& P){
  //(x,y) = theta/π · (sin(πr), cos(πr))
  float x = P.x, y = P.y;
  //std::cout << "x = " << x << " and y = " << y << std::endl;
  float theta = atan2(x,y);
  float r = sqrt((x*x)+(y*y));
  float pi = 3.14159265;
  P.x = (theta/pi) * (sin(pi * r));
  P.y = (theta/pi) * (cos(pi * r));
  return 0.8;
}

void ImgProc::biasUp(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] + 0.15; }
}

void ImgProc::biasDown(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] - 0.15; }
}

void ImgProc::rmsContrast(){
   //create variables to hold sums, means, and standard devations
   //x and y variables are just in case we have more than 3 channels (RGB)
   float rSum = 0, gSum = 0, bSum = 0, xSum = 0, ySum = 0;
   float rMean = 0, gMean = 0, bMean = 0, xMean = 0, yMean = 0;
   float rDev = 0, gDev = 0, bDev = 0, xDev = 0, yDev = 0;
   if( img_data == nullptr ){ return; }

   //gather sums of rgb values
   for( long i=0;i<Nsize;i+=Nc ){
     rSum += img_data[i];
     gSum += img_data[i+1];
     bSum += img_data[i+2];
     if(Nc>3){ //if we have an image with more than 3 channels, gather sums of other channels
       xSum += img_data[i+3];
       if(Nc>4){
         ySum += img_data[i+4];
       }
     }
   }
   //calculate r, g, and b means
   rMean = rSum/Nsize;
   gMean = gSum/Nsize;
   bMean = bSum/Nsize;
   if(Nc>3){ //if we have an image with more than 3 channels, calculate means of other channels
     xMean = xSum/Nsize;
     if(Nc>4){
       yMean = ySum/Nsize;
     }
   }
   //calculate r, g, and b standard devations
   rSum = gSum = bSum = 0;
   for( long i=0;i<Nsize;i+=Nc ){
     rSum += pow(img_data[i] - rMean, 2);
     gSum += pow(img_data[i+1] - gMean, 2);
     bSum += pow(img_data[i+2] - bMean, 2);
     if(Nc>3){ //if we have an image with more than 3 channels, calculate std devs of other channels (squared)
       xSum += pow(img_data[i+3] - xMean, 2);
       if(Nc>4){
         ySum += pow(img_data[i+4] - yMean, 2);
       }
     }
   }
   rDev = sqrt(rSum/Nsize);
   gDev = sqrt(gSum/Nsize);
   bDev = sqrt(bSum/Nsize);
   if(Nc>3){ //if we have an image with more than 3 channels, take square root of the other channels' squared standard devations
     xDev = sqrt(xSum/Nsize);
     if(Nc>4){
       yDev = sqrt(ySum/Nsize);
     }
   }

   //finally, set values
#pragma omp parallel for
   for( long i=0;i<Nsize;i+=Nc ){
     img_data[i] = ((img_data[i]-rMean)/rDev);
     img_data[i+1] = ((img_data[i+1]-gMean)/gDev);
     img_data[i+2] = ((img_data[i+2]-bMean)/bDev);
     if(Nc>3){
       img_data[i+3] = ((img_data[i+3]-xMean)/xDev);
       if(Nc>4){
         img_data[i+4] = ((img_data[i+4]-yMean)/yDev);
       }
     }
   }
}

void ImgProc::gammaUp(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = pow(img_data[i], 1.8); }
}

void ImgProc::gammaDown(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = pow(img_data[i], 0.5); }
}

void ImgProc::writeOpenEXR(){
   if( img_data == nullptr ){ return; }
   const char *filename = "image.exr";
   ImageOutput *out = ImageOutput::create(filename);
   if(!out){return;}
   ImageSpec spec(Nx, Ny, Nc, TypeDesc::FLOAT);
   out->open(filename, spec);
   out->write_image(TypeDesc::FLOAT, img_data);
   out->close();
   ImageOutput::destroy(out);
}

void ImgProc::quantize(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i+=Nc ){
     //declare rgb values, x and y values are in case we have up to 5 channels
     float r = 0, g = 0, b = 0, x = 0, y = 0;
     r = (int)(img_data[i]*5);
     r = (float)(r / 5);
     g = (int)(img_data[i+1]*5);
     g = (float)(g / 5);
     b = (int)(img_data[i+2]*5);
     b = (float)(b / 5);
     img_data[i] = r;
     img_data[i+1] = g;
     img_data[i+2] = b;
     if(Nc>3){ //if we an image with more than 3 channels (rgb), then perform quantize on other channels
       x = (int)(img_data[i+3]*5);
       x = (float)(x / 5);
       img_data[i+3] = x;
     }
     else if(Nc>4){
       y = (int)(img_data[i+4]*5);
       y = (float)(y / 5);
       img_data[i+4] = y;
     }
   }
}

void ImgProc::brightnessUp(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] * 1.25; }
}

void ImgProc::brightnessDown(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] * 0.75; }
}

void ImgProc::grayscale(){
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i+=Nc ){
     float r = 0, g = 0, b = 0, x = 0;
     r = img_data[i];
     g = img_data[i+1];
     b = img_data[i+2];
     x = ((0.2126*r) + (0.7152*g) + (0.0722*b));
     img_data[i] = x;
     img_data[i+1] = x;
     img_data[i+2] = x;
   }
}

void ImgProc::histogramEQ(){
   if( img_data == nullptr ){ return; }
   float Imin, Imax, Idelta;
   float Ieq, Q, w;
   int q, m, N = 256; //N is # of bins

//loop through each channel, independently applying histogramEQ
#pragma omp parallel for
   for( long i=0;i<Nc;i++ ){
     //CREATE HISTOGRAM, PDF, AND CDF//////////////////////////
     //get min, max, and delta for this channel
     Imin = Iminimum(i);
     Imax = Imaximum(i);
     Idelta = (Imax - Imin) / (N-1);
     std::vector<int> histogram(N, 0);
     std::vector<float> PDF(N, 0.0);
     std::vector<float> CDF(N, 0.0);

     for( long j=i;j<Nsize;j+=Nc ){
       m = (int)((img_data[j] - Imin) / Idelta);
       if(m>=N){m=N-1;}
       if(m<0){m=0;}
       histogram[m]++;
     }
     //then PDF is simply histogram[i] / Nx*Ny
     for( long j=0;j<N;j++ ){
       PDF[j] = ((float)histogram[j] / (float)(Nx*Ny));
     }
     //and CDF is CDF[0] = PDF[0], CDF[q] = CDF[q-1] + PDF[q]
     for( long j=0;j<N;j++ ){
       if(j==0){CDF[0] = PDF[0];}
       else if(j>0){CDF[j] = CDF[j-1] + PDF[j];}
     }
     //HISTOGRAM, PDF, AND CDF CREATION COMPLETE//////////////

     //IMPLEMENT HISTOGRAM EQUALIZATION///////////////////////
     for( long j=i;j<Nsize;j+=Nc ){
       Q = (img_data[j] - Imin) / Idelta;
       q = (int)Q;
       w = (Q-q);
       if(q<N-1){
         Ieq = ( CDF[q] * (1-w) ) + ( CDF[q+1] * w);
       }
       else if(q==(N-1)){
         Ieq = CDF[q];
       }
       else{Ieq = 0;}

       //replace Ii with Ieq
       img_data[j] = Ieq;
     }
    //HISTOGRAM EQUALIZATION COMPLETE FOR THIS CHANNEL//////
   }
}

void ImgProc::pixelAnalysis(){
   if( img_data == nullptr ){ return; }
   float rMin, rMax, rMean, rStdDev;
   float gMin, gMax, gMean, gStdDev;
   float bMin, bMax, bMean, bStdDev;

   //call all these functions for each channel and set equal to the variable for that channel
   //R
   rMin = Iminimum(0);
   rMax = Imaximum(0);
   rMean = Iaverage(0);
   rStdDev = IstdDev(0);
   std::cout << "rMin: " << rMin << ", rMax: " << rMax << ", rMean: " << rMean << ", rStdDev: " << rStdDev << std::endl;
   //G
   gMin = Iminimum(1);
   gMax = Imaximum(1);
   gMean = Iaverage(1);
   gStdDev = IstdDev(1);
   std::cout << "gMin: " << gMin << ", gMax: " << gMax << ", gMean: " << gMean << ", gStdDev: " << gStdDev << std::endl;
   //B
   bMin = Iminimum(2);
   bMax = Imaximum(2);
   bMean = Iaverage(2);
   bStdDev = IstdDev(2);
   std::cout << "bMin: " << bMin << ", bMax: " << bMax << ", bMean: " << bMean << ", bStdDev: " << bStdDev << std::endl;
   std::cout << "All done!" << std::endl;
}

float ImgProc::Iminimum(int x){
  float min = img_data[x];
  //#pragma omp parallel for
     for( long i=x;i<(Nx*Ny);i+=Nc ){
       if(img_data[i+Nc] < min){min = img_data[i+Nc];}
     }
  return min;
}

float ImgProc::Imaximum(int x){
  float max = img_data[x];
  //#pragma omp parallel for
     for( long i=x;i<(Nx*Ny);i+=Nc ){
       if(img_data[i+Nc] > max){max = img_data[i+Nc];}
     }
  return max;
}

float ImgProc::Iaverage(int x){
  float sum = 0;
  //#pragma omp parallel for
     for( long i=x;i<(Nx*Ny);i+=Nc ){
       sum += img_data[i];
     }
  return (sum/(Nx*Ny));
}

float ImgProc::IstdDev(int x){
  float stdDev = 0;
  float Iavg = Iaverage(x);
  //calculate r, g, and b standard devations
  for( long i=x;i<(Nx*Ny);i+=Nc ){
    stdDev += pow(img_data[i] - Iavg, 2);
  }
  stdDev = sqrt(stdDev/(Nx*Ny));
  return stdDev;
}

//END ADDED CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
