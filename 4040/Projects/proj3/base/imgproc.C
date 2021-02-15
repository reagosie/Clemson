
#include <cmath>
#include <math.h>
#include <time.h>
#include <vector>
#include "imgproc.h"

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

using namespace img;

ImgProc::ImgProc() :
  Nx (0),
  Ny (0),
  Nc (0),
  Nsize (0),
  img_data (nullptr)
{}

ImgProc::~ImgProc()
{
   clear();
}

void ImgProc::clear()
{
   if( img_data != nullptr ){ delete[] img_data; img_data = nullptr;}
   Nx = 0;
   Ny = 0;
   Nc = 0;
   Nsize = 0;
}

void ImgProc::clear(int nX, int nY, int nC)
{
   clear();
   Nx = nX;
   Ny = nY;
   Nc = nC;
   Nsize = (long)Nx * (long)Ny * (long)Nc;
   img_data = new float[Nsize];
#pragma omp parallel for
   for(long i=0;i<Nsize;i++){ img_data[i] = 0.0; }
}

bool ImgProc::load( const std::string& filename )
{
   auto in = ImageInput::create (filename);
   if (!in) {return false;}
   ImageSpec spec;
   in->open (filename, spec);
   clear();
   Nx = spec.width;
   Ny = spec.height;
   Nc = spec.nchannels;
   Nsize = (long)Nx * (long)Ny * (long)Nc;
   img_data = new float[Nsize];
   in->read_image(TypeDesc::FLOAT, img_data);
   in->close ();
   return true;
}


void ImgProc::value( int i, int j, std::vector<float>& pixel) const
{
   pixel.clear();
   if( img_data == nullptr ){ return; }
   if( i<0 || i>=Nx ){ return; }
   if( j<0 || j>=Ny ){ return; }
   pixel.resize(Nc);
   for( int c=0;c<Nc;c++ )
   {
      pixel[c] = img_data[index(i,j,c)];
   }
   return;
}

void ImgProc::set_value( int i, int j, const std::vector<float>& pixel)
{
   if( img_data == nullptr ){ return; }
   if( i<0 || i>=Nx ){ return; }
   if( j<0 || j>=Ny ){ return; }
   if( Nc > (int)pixel.size() ){ return; }
#pragma omp parallel for
   for( int c=0;c<Nc;c++ )
   {
      img_data[index(i,j,c)] = pixel[c];
   }
   return;
}



ImgProc::ImgProc(const ImgProc& v) :
  Nx (v.Nx),
  Ny (v.Ny),
  Nc (v.Nc),
  Nsize (v.Nsize)
{
   img_data = new float[Nsize];
#pragma omp parallel for
   for( long i=0;i<Nsize;i++){ img_data[i] = v.img_data[i]; }
}

ImgProc& ImgProc::operator=(const ImgProc& v)
{
   if( this == &v ){ return *this; }
   if( Nx != v.Nx || Ny != v.Ny || Nc != v.Nc )
   {
      clear();
      Nx = v.Nx;
      Ny = v.Ny;
      Nc = v.Nc;
      Nsize = v.Nsize;
   }
   img_data = new float[Nsize];
#pragma omp parallel for
   for( long i=0;i<Nsize;i++){ img_data[i] = v.img_data[i]; }
   return *this;
}


void ImgProc::operator*=(float v)
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] *= v; }
}

void ImgProc::operator/=(float v)
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] /= v; }
}

void ImgProc::operator+=(float v)
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] += v; }
}

void ImgProc::operator-=(float v)
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] -= v; }
}


void ImgProc::compliment()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = 1.0 - img_data[i]; }
}

//START ADDED CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
   float rSum = 0, gSum = 0, bSum = 0;
   float rMean = 0, gMean = 0, bMean = 0;
   float rDev = 0, gDev = 0, bDev = 0;
   if( img_data == nullptr ){ return; }

   //gather sums of rgb values
   for( long i=0;i<(Nx*Ny);i+=Nc ){
     rSum += img_data[i];
     gSum += img_data[i+1];
     bSum += img_data[i+2];
   }
   //calculate r, g, and b means
   rMean = rSum/(Nx*Ny);
   gMean = gSum/(Nx*Ny);
   bMean = bSum/(Nx*Ny);
   //calculate r, g, and b standard devations
   rSum = gSum = bSum = 0;
   for( long i=0;i<(Nx*Ny);i+=Nc ){
     rSum += pow(img_data[i] - rMean, 2);
     gSum += pow(img_data[i+1] - gMean, 2);
     bSum += pow(img_data[i+2] - bMean, 2);
   }
   rDev = sqrt(rSum/(Nx*Ny));
   gDev = sqrt(gSum/(Nx*Ny));
   bDev = sqrt(bSum/(Nx*Ny));

   //finally, set values
#pragma omp parallel for
   for( long i=0;i<(Nx*Ny);i+=Nc ){
     img_data[i] = ((img_data[i]-rMean)/rDev);
     img_data[i+1] = ((img_data[i+1]-gMean)/gDev);
     img_data[i+2] = ((img_data[i+2]-bMean)/bDev);
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
   for( long i=0;i<(Nx*Ny);i+=Nc ){
     float r = 0, g = 0, b = 0;
     r = (int)(img_data[i]*5);
     r = (float)(r / 5);
     g = (int)(img_data[i+1]*5);
     g = (float)(g / 5);
     b = (int)(img_data[i+2]*5);
     b = (float)(b / 5);
     img_data[i] = r;
     img_data[i+1] = g;
     img_data[i+2] = b;
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
   for( long i=0;i<(Nx*Ny);i+=Nc ){
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

long ImgProc::index(int i, int j, int c) const
{
   return (long) c + (long) Nc * index(i,j); // interleaved channels

   // return index(i,j) + (long)Nx * (long)Ny * (long)c; // sequential channels
}

long ImgProc::index(int i, int j) const
{
   return (long) i + (long)Nx * (long)j;
}



void img::swap(ImgProc& u, ImgProc& v)
{
   float* temp = v.img_data;
   int Nx = v.Nx;
   int Ny = v.Ny;
   int Nc = v.Nc;
   long Nsize = v.Nsize;

   v.Nx = u.Nx;
   v.Ny = u.Ny;
   v.Nc = u.Nc;
   v.Nsize = u.Nsize;
   v.img_data = u.img_data;

   u.Nx = Nx;
   u.Ny = Ny;
   u.Nc = Nc;
   u.Nsize = Nsize;
   u.img_data = temp;
}
