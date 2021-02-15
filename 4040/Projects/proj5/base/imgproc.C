
#include "imgproc.h"
#include "fftanalysis.h"
#include "fftw3.h"
#include <iostream>
#include <cmath>
#include <math.h>
#include <time.h>
#include <vector>

#include <GL/gl.h>   // OpenGL itself.
#include <GL/glu.h>  // GLU support library.
#include <GL/glut.h> // GLUT support library.

#include <OpenImageIO/imageio.h>
OIIO_NAMESPACE_USING

using namespace img;


///START IMGPROC FUNCTIONS////////////////////////////////////////////////////////////////////////////////////////
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

void ImgProc::compliment()
{
   if( img_data == nullptr ){ return; }
#pragma omp parallel for
   for( long i=0;i<Nsize;i++ ){ img_data[i] = 1.0 - img_data[i]; }
}

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
///END IMGPROC FUNCTIONS//////////////////////////////////////////////////////////////////////////////////////////








///START FFT FUNCTIONS///////////////////////////////////////////////////////////////////////////////////////////
FFTImgProc::FFTImgProc() :
   Nx(0),
   Ny(0),
   Nc(0),
   img_data(nullptr)
{}

FFTImgProc::~FFTImgProc(){ clear(); }

void FFTImgProc::clear()
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
   Nc = 0;
   Nsize = 0;
}

void FFTImgProc::clear(int nX, int nY, int nC)
{
   clear();
   Nx = nX;
   Ny = nY;
   Nc = nC;
   Nsize = Nx*Ny*Nc;
   img_data = (fftw_complex*)fftw_malloc( sizeof(fftw_complex)*Nx*Ny*Nc );
#pragma omp parallel for
   for(long i=0;i<Nsize;i++){ img_data[i][0] = 0.0;  img_data[i][1] = 0.0; }

   forward.resize(Nc);
   backward.resize(Nc);
   for(int i=0;i<Nc;i++)
   {
      forward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_FORWARD, FFTW_ESTIMATE);
      backward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}

void FFTImgProc::value( int i, int j, std::vector<std::complex<double> >& pixel) const
{
   if( img_data == nullptr ){ return; }
   if( i<0 || i>=Nx ){ return; }
   if( j<0 || j>=Ny ){ return; }
   pixel.resize(Nc);
   for( int c=0;c<Nc;c++ )
   {
      const fftw_complex& v = img_data[index(i,j,c)];
      std::complex<double> a(v[0],v[1]);
      pixel[c] = a;
   }
   return;
}



FFTImgProc::FFTImgProc(const FFTImgProc& v) :
  Nx (v.Nx),
  Ny (v.Ny),
  Nc (v.Nc),
  Nsize (v.Nsize)
{
   img_data = (fftw_complex*)fftw_malloc( sizeof(fftw_complex)*Nx*Ny*Nc );
#pragma omp parallel for
   for(long i=0;i<Nsize;i++){ img_data[i][0] = v.img_data[i][0];  img_data[i][1] = v.img_data[i][1]; }

   forward.resize(Nc);
   backward.resize(Nc);
   for(int i=0;i<Nc;i++)
   {
      forward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_FORWARD, FFTW_ESTIMATE);
      backward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_BACKWARD, FFTW_ESTIMATE);
   }
}

FFTImgProc& FFTImgProc::operator=(const FFTImgProc& v)
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

   img_data = (fftw_complex*)fftw_malloc( sizeof(fftw_complex)*Nx*Ny*Nc );
#pragma omp parallel for
   for(long i=0;i<Nsize;i++){ img_data[i][0] = v.img_data[i][0];  img_data[i][1] = v.img_data[i][1]; }

   forward.resize(Nc);
   backward.resize(Nc);
   for(int i=0;i<Nc;i++)
   {
      forward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_FORWARD, FFTW_ESTIMATE);
      backward[i] = fftw_plan_dft_2d( Ny, Nx, &img_data[index(0,0,i)], &img_data[index(0,0,i)], FFTW_BACKWARD, FFTW_ESTIMATE);
   }
   return *this;
}


void FFTImgProc::set_value( int i, int j, const std::vector<std::complex<double> >& pixel)
{
   if( img_data == nullptr ){ return; }
   if( i<0 || i>=Nx ){ return; }
   if( j<0 || j>=Ny ){ return; }
   if( pixel.size() != (size_t)Nc ){ return; }
   for( int c=0;c<Nc;c++ )
   {
      fftw_complex& v = img_data[index(i,j,c)];
      v[0] = pixel[c].real();
      v[1] = pixel[c].imag();
   }
   return;
}



void FFTImgProc::fft_forward()
{
   for(size_t i=0;i<forward.size();i++){ fftw_execute(forward[i]); }
}

void FFTImgProc::fft_backward()
{
   for(size_t i=0;i<backward.size();i++){ fftw_execute(backward[i]); }
   double norm = 1.0/(Nx*Ny);
   for(int j=0;j<Ny;j++)
   {
#pragma omp parallel for
      for(int i=0;i<Nx;i++)
      {
         for(int c=0;c<Nc;c++)
	 {
	    img_data[index(i,j,c)][0] *= norm;
	    img_data[index(i,j,c)][1] *= norm;
	 }
      }
   }
}

double FFTImgProc::kx(int i) const
{
   double v = (double)i/Nx;
   if(i>Nx/2){ v -= 1.0; }
   return 2.0*3.14159265*v;
}

double FFTImgProc::ky(int j) const
{
   double v = (double)j/Ny;
   if(j>Ny/2){ v -= 1.0; }
   return 2.0*3.14159265*v;
}


long FFTImgProc::index(int i, int j, int c) const
{
   // return (long) c + (long) Nc * index(i,j); // interleaved channels

   return index(i,j) + (long)Nx * (long)Ny * (long)c; // sequential channels
}

long FFTImgProc::index(int i, int j) const
{
   return (long) i + (long)Nx * (long)j;
}
//////END FFT FUNCTIONS///////////////////////////////////////////////////////////////////////////////////////////////////////





////LINEAR WAVE ESTIMATION FUNCTIONS/////////////////////////////////////////////////////////////////////////////////
//START ADDED CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LinearWaveEstimate::LinearWaveEstimate(const ImgProc& init, const double dispersion_factor) :
  alpha(dispersion_factor),
  frame_count(0){
    A.clear(init.nx(), init.ny(), init.depth());
    B.clear(init.nx(), init.ny(), init.depth());
  }

double LinearWaveEstimate::dispersion(double kx, double ky) const {
  double kmag = std::sqrt( kx*kx + ky*ky );
  double freq = alpha * std::sqrt(kmag);
  return freq;
}

void LinearWaveEstimate::ingest(const ImgProc& I){
  FFTImgProc Itilde;
  img::load_fft(I, Itilde);
  Itilde.fft_forward();

  for(int j = 0; j < Itilde.ny(); j++){
#pragma omp parallel for
    for(int i = 0; i < Itilde.nx(); i++){
      std::vector< std::complex<double> > itilde;
      std::vector< std::complex<double> > a;
      std::vector< std::complex<double> > b;
      Itilde.value(i, j, itilde);
      A.value(i, j, a);
      B.value(i, j, b);
      std::vector< std::complex<double> > aupdate = a;
      std::vector< std::complex<double> > bupdate = b;
      std::complex<double> phase(0.0, frame_count * dispersion(Itilde.kx(i), Itilde.ky(j)));
      phase = std::exp(phase);
      double one_over_N = 1.0/(frame_count + 1);
      for(size_t c = 0; c < itilde.size(); c++){
        aupdate[c] += (itilde[c]/phase - b[c]/(phase*phase) - a[c] )*one_over_N;
        bupdate[c] += (itilde[c]*phase - a[c]*(phase*phase) - b[c] )*one_over_N;
      }
      A.set_value(i, j, aupdate);
      B.set_value(i, j, bupdate);
    }
  }
  frame_count++;
}

void LinearWaveEstimate::value(int i, int j, int n, std::vector< std::complex<double> >& amplitude) const{
  std::complex<double> phase(0.0, n * dispersion(A.kx(i),A.ky(j)));
  phase = std::exp(phase);
  std::vector< std::complex<double> > a;
  std::vector< std::complex<double> > b;
  A.value(i, j, a);
  B.value(i, j, b);
  amplitude.resize( a.size() );
  for(size_t c = 0; c < a.size(); c++){
    amplitude[c] = a[c]*phase + b[c]/phase;
  }
}

void img::extract_image(const LinearWaveEstimate& l, int frame, ImgProc& img){
  img.clear(l.getA().nx(), l.getA().ny(), l.getA().depth());

  FFTImgProc fftimg;
  fftimg.clear(img.nx(), img.ny(), img.depth());
  for(int j = 0; j < img.ny(); j++){
    for(int i = 0; i < img.nx(); i++){
      std::vector<std::complex<double> > v;
      l.value(i, j, frame, v);
      fftimg.set_value(i, j, v);
    }
  }

  fftimg.fft_backward();
  for(int j = 0; j < img.ny(); j++){
    for(int i = 0; i < img.nx(); i++){
      std::vector< std::complex<double> > v;
      fftimg.value(i, j, v);
      std::vector<float> iv(v.size());
      for(size_t c = 0; c < v.size(); c++){
        iv[c] = v[c].real();
      }
      img.set_value(i, j, iv);
    }
  }
}
//END ADDED CODE/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
