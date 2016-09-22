#include "Diff_Filter.h"
#include <iostream>
#include <algorithm>
#include <vector> 

using namespace imaging;

Image * DiffFilter::apply( Image & src)
{
	std:: cout << "Applying Diff Filter...\n";
	std:: vector<float> vred;
	std:: vector<float> vgreen;
	std:: vector<float> vblue;

	Image * newImage = new Image(src);
	
	
	for (unsigned int i =0 ; i< src.getHeight();i++){
		for(unsigned int j=0 ; j<src.getWidth(); j++){
			int count = 0; 
			float red = 0;
			float green = 0;
			float blue =0;
			
			Color c1;
			Color c2;
			for(int m = -1 ; m<=1;m++){
				for(int n = -1; n<=1 ; n++){

					if((i+m<src.getHeight())&&((int)i+m>=0)&&(j+n<src.getWidth())&&((int)j+n>=0)){
						
						count++;
						c1=src.getPixel((int)i+m,(int)j+n);
						
						red=(float)c1.x;
						green=(float)c1.y;
						blue=(float)c1.z;
					
						vred.push_back(red);
						vgreen.push_back(green);
						vblue.push_back(blue);
					}
				}
			}
			
			
			std:: sort (vred.begin(),vred.end());
			std:: sort (vgreen.begin(),vgreen.end());
			std:: sort (vblue.begin(),vblue.end());
			
		
			c2.x=vred[vred.size()-1]-vred[0];
			c2.y=vgreen[vgreen.size()-1]-vgreen[0];
			c2.z=vblue[vblue.size()-1]-vblue[0];
			newImage->setPixel(i,j,c2);

			vred.clear();
			vblue.clear();
			vgreen.clear();
			}
		}

	std:: cout << "The filter was succesfully applied\n"; 
	return newImage;
}