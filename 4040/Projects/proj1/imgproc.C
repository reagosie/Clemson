
#include <cmath>
#include <math.h>
#include "imgproc.h"

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

void ImgProc::biasUp()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] + 0.15; }
}

void ImgProc::biasDown()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] - 0.15; }
}

void ImgProc::rmsContrast()
{
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
   rSum = gSum = bSum = xSum = ySum = 0;
   //calculate r, g, and b standard devations (squared)
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
   //take square root of the squared standard devations
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

void ImgProc::gammaUp()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = pow(img_data[i], 1.5); }
}

void ImgProc::gammaDown()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = pow(img_data[i], 0.5); }
}

void ImgProc::writeOpenEXR()
{
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

void ImgProc::quantize()
{
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

void ImgProc::brightnessUp()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] * 1.25; }
}

void ImgProc::brightnessDown()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = img_data[i] * 0.75; }
}

void ImgProc::grayscale()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i+=3 ){
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
