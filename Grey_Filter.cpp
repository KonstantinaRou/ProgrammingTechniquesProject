#include "Grey_Filter.h"
#include <string>
#include <cstdlib>
#include <iostream >

using namespace imaging;

Image * GreyFilter::apply(Image & src)
{
	std:: cout << "\n Applying gray filter...\n"; 
	Color color;
	unsigned int m;

	Image *newImage=new Image(src);
	
	for(unsigned int j = 0 ; j<src.getHeight(); j++){
		for(unsigned int i = 0 ; i <src.getWidth() ; i++){
			color =src.getPixel(j,i);
			m = (unsigned int)((float)color.x+ (float)color.y+ (float)color.z)/3 ;
			color.x=(Component)m;
			color.y=(Component)m;
			color.z=(Component)m;
			newImage->setPixel(j,i,color);
		}
	}
	std:: cout << "The filter was succesfully applied\n"; 
	return newImage;
}
