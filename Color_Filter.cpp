#include "Color_Filter.h"
#include <string>
#include <cstdlib>
#include <iostream>

using namespace imaging;

Image * ColorFilter::apply( Image & src)
{
	std:: cout << "Applying Color Filter...\n";
	
	Image *newImage=new Image(src);

	float red , green , blue ;
	Color cl = getColor();;
	Color colorr;

	 red = ((float)cl.x)/255.0f;
	 green =  ((float)cl.y)/255.0f;
	 blue  = ((float)cl.z)/255.0f; 

	 for (unsigned int i = 0; i<src.getHeight(); i++){
		 for (unsigned int j=0; j<src.getWidth();j++){

			colorr=src.getPixel(i,j);
			colorr.x=(Component)(red*(float)colorr.x);
			colorr.y=(Component)(green*(float)colorr.y);
			colorr.z=(Component)(blue*(float)colorr.z);
			newImage->setPixel(i,j,colorr);


		 }
	 }
	std:: cout << "The filter was succesfully applied\n"; 
	return newImage;
}
