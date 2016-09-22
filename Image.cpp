#include "Image.h"
#include <algorithm>
#include <iostream>

namespace imaging 
{

//-------------------------------- Image class implementation goes here ---------------------------------------------------
	
	Image::Image(unsigned int width, unsigned int height, const Component * data_ptr,bool interlavated){
		Image::width = width;
		Image::height = height;
		Image::buffer = (Component *) calloc(3*width*height,sizeof(Component));
		setData(data_ptr);
		Image::interleaved=interlavated;
		
	}

	Image::Image(unsigned int width, unsigned int height,bool interlavated){
		Image::width = width;
		Image::height = height;
		Image::buffer = (Component *) calloc(3*width*height,sizeof(Component));
		Image::interleaved=interlavated;
	}

	Image::Image( const Image &src){
		Image::width = src.getWidth();
		Image::height = src.getHeight();
		Image::interleaved = src.isInterleaved();
		Image::buffer = (Component *) calloc(3*width*height,sizeof(Component));
		for (unsigned int i=0; i<getHeight()*getWidth()*3 ; i++) {
			Image::buffer[i]=src.getRawDataPtr()[i];
		}
	}

	Image::~Image(){
		free(buffer);
	}

	/* -----------------setPixel method : epistrefei to xrwma-----------------------------------
	   1. elegxoume an h eisodos einai mesa sta oria ths eikonas
	   2. elegxoume an h eikona h opoia kalei thn getPixel einai ppm h cpi mesw tou interleaved
	   3. an einai ppm, vriskoume tis theseis pou analagoun sto kokkino prasino kai mple xrwma apo ton pinaka ths kalousas eikonas
	   4. pername ta ston pinaka dedomenwn ths eikonas stis theseis pou exoume vrei ta R G B ta stoixeia tou xrwmatos 
		  pou exoume perasei san orisma ths synartishs. 
	   5. anistoixa kai sthn periptwsh pou einai cpi format h kalousa eikona */

	void Image::setPixel(unsigned int x , unsigned int y, Color value){
		if(y < getWidth() && x < getHeight()){
			int red,green,blue;

			
			if ( isInterleaved() ){ // an einai ppm
				 red = 3*(x*getWidth()+y);
				 green = 3*(x*getWidth()+y)+1;
				 blue = 3*(x*getWidth()+y)+2;
			}

			else {					// an einai cpi
				
				 red = (x*getWidth()+y);
				 green = red + getWidth()*getHeight();
				 blue = green + getWidth()*getHeight();
			}
			
			Image::buffer[red]=value.x;
			Image::buffer[green]=value.y;
			Image::buffer[blue]=value.z;
		}
		
	}
	
	/* -----------------getPixel method : epistrefei to xrwma-----------------------------------
	   1. elegxoume an h eisodos einai mesa sta oria ths eikonas
	   2. elegxoume an h eikona h opoia kalei thn getPixel einai ppm h cpi mesw tou interleaved
	   3. an einai ppm, vriskoume tis theseis pou analagoun sto kokkino prasino kai mple xrwma apo ton pinaka ths kalousas eikonas
	   4. se mia metavliti color pername tis theseis tou pinaka 
	   5. epistrefoume to xrwma
	   6. anistoixa kai sthn periptwsh pou einai cpi format h kalousa eikona */
	
	Color Image:: getPixel(unsigned int x, unsigned int y) const {
		if(y < getWidth() && x < getHeight()){
			if ( isInterleaved() ){ // einai ppm
				int red = 3*(x*getWidth()+y);
				int green =3*(x*getWidth()+y)+1;
				int blue = 3*(x*getWidth()+y)+2;
				
				Color *cl = new Color();
				cl->x=Image::buffer[red];
				cl->y=Image::buffer[green];
				cl->z=Image::buffer[blue];
				return * cl ;
				
			}
			else {             // einai cpi 
				int red = (x*getWidth()+y);
				int green = red + getWidth()*getHeight();
				int blue = green + getWidth()*getHeight();
				
				Color *cl = new Color();
				cl->x=Image::buffer[red];
				cl->y=Image::buffer[green];
				cl->z=Image::buffer[blue];
				
				return * cl ;
			}
		}
		return *(new Color()) ; //den tha ftassei pote edw
	}

	
	/*-------setData method : antigrafi tou pinaka dedomenvwn ston pinaka buffer--------*/

	void Image::setData(const Component * & data_ptr){
		for (unsigned int i=0; i<getHeight()*getWidth()*3 ; i++) {
				Image::buffer[i]=data_ptr[i];
			}
	}
	
	/*-------------- convertToColorPlaneImage method : metatropi eikonas se cpi format-----------------

	1.dimiourgoume mia prosorinh eikona temp sthn  opoia to orisma interleaved =false (cpi)
	2.Gia kathe pixel tis eikonas vriskoume thn thesi twn RGB ston pinaka ths eikonas me thn getPixel 
	  kai epistrefoyme to xrwma cl
	3.to xrwma to vazoyme ston monodiastato pinaka tou temp me tin setPixel
	4."antigrafoume" ton monodiastato tropopoihmeno pleon pinaka tou temp ston pinaka ths arxikhs eikonas */
	
	void Image::convertToColorPlaneImage(){
		
			Image * temp = new Image(getWidth(),getHeight(),false);
			Color cl;
			
			for( unsigned int i = 0; i<getHeight();i++){
				for(unsigned int j=0; j<getWidth();j++){
					cl=this->getPixel(i,j);
					temp->setPixel(i,j,cl);
					
				}
			}
			const Component * a = temp->getRawDataPtr();
			setData(a);
			Image::interleaved = false;
		}
	

	/*-------------- convertToInterleavedImage method : metatropi eikonas se ppm format-----------------

	1.dimiourgoume mia prosorinh eikona temp sthn  opoia to orisma interleaved =true (ppm)
	2.Gia kathe pixel tis eikonas vriskoume thn thesi twn RGB ston pinaka ths eikonas me thn getPixel 
	  kai epistrefoyme to xrwma cl
	3.to xrwma to vazoyme ston monodiastato pinaka tou temp me tin setPixel
	4."antigrafoume" ton monodiastato tropopoihmeno pleon pinaka tou temp ston pinaka ths arxikhs eikonas */
	
	void Image::convertToInterleavedImage(){
		Image * temp = new Image(getWidth(),getHeight(),true);
		Color cl;
		
		for( unsigned int i = 0; i<getHeight();i++){
				for(unsigned int j=0; j<getWidth();j++){
					cl=this->getPixel(i,j);
					temp->setPixel(i,j,cl);
					
				}
			}
			const Component * a = temp->getRawDataPtr();
			setData(a);
			Image::interleaved = true;
	}
	
	
	
//-------------------------------- Helper functions -----------------------------------------------------------------------

	bool isSuffix(std::string & filename, const std::string suffix)
	{
		std::string low_caps_filename = filename;
		std::string dot_suffix = '.' + suffix;
		
		// make the string lowercase.
		std::transform(low_caps_filename.begin(), low_caps_filename.end(), low_caps_filename.begin(), ::tolower );

		//make the suffix lowercase.
		std::transform(dot_suffix.begin(), dot_suffix.end(), dot_suffix.begin(), ::tolower );
		
		size_t pos = low_caps_filename.rfind(dot_suffix);
		
		if (pos == std::string::npos) // did not find it
			return false;

		if (pos < low_caps_filename.length()-dot_suffix.length()) // found it, but not at the end
			return false;
		
		return true; // now we are positive. :)
	}

	void replaceSuffix(std::string & filename, const std::string suffix)
	{
		size_t pos = filename.rfind('.');
		if (pos==std::string::npos)
			return;

		filename.resize(pos+1);
		filename+=suffix;
	}


} // namespace imaging