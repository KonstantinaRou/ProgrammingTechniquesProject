#include "rle_format.h"
#include "Block.h"
#include <iostream>
#include <fstream>
#include <vector>


using namespace imaging;

void RLEImageWriter :: write(std::string filename, const Image & src) {
	std::ofstream file;

	if (isSuffix(filename,"cpi") || isSuffix(filename,"ppm"))
		replaceSuffix(filename,"rle");

	file.open(filename, std::ios::binary| std::ios::out |std::ios::trunc );
	
	if(!file.is_open()){ 
		std::cout << "Counld not open file \n"; // mhnyma lathoys 
		addLogEntry("Counld not open file \n");
	}

	if (file != NULL ){
		
		//xrisimopoioume thn methodo file pou vrisketai sto ofstream 
		//gia na prosdiorisoume emeis ta bytes
		file.write("CPI", 3);
		
		unsigned char two = 2;
		file.write((char*)&two, 1);
		
		unsigned short endian=258;
		file.write((char*)&endian, sizeof(endian));
		
		unsigned short w = src.getWidth();
		file.write((char*)&w,2);

		unsigned short h = src.getHeight();
		file.write((char*)&h,2);

		file.write((char*)&block_length,2);

		Image * myImage = new Image (src);
		
		int n_blocks = (int)ceil( w / (float)block_length );
		int b_last = w-(n_blocks-1)*block_length;

		ivec2 * pos = new ivec2(0,0);
		
		Block *temp;
		for(unsigned int channel=myImage->RED;channel<=myImage->BLUE;channel++)
		{
			for(unsigned int i=0; i< src.getHeight(); i++){
				for (unsigned int j=0; j<n_blocks;j++){
				
				
					temp = new Block(block_length);
					temp->setErrorMargin(threshold);
					temp=temp->copyFromImage(*myImage,(Image::channel_t)channel,*pos,block_length);
					//---------------------------------------------------------------------------
					unsigned int c1=0;
					while(c1<temp->getSize()){
						unsigned int c2=1;
						Component val = temp->getDataPtr()[c1];
						
						c1++;
						while(c1<temp->getSize()&&abs(temp->getDataPtr()[c1]-val)<threshold){
							c2++;
							c1++;
						}
			
						file.write((char*)&val,1);
						file.write((char*)&c2,1);
						
					}
					pos->x+=block_length;
				}
				pos->y=i;
				pos->x=0;
			}
		}
	}


	else{
		std::cout << "Cannot open file :";
		std:: cout << filename ;
		std:: cout << "\n It might be open from an other application\n";
		
	}
	file.close();
}

Image * RLEImageReader:: read (std::string filename){

	std:: vector <Component> tmp;
	std:: ifstream file ;
	file.open(filename, std::ios::binary | std::ios::in); 

	if(!file.is_open()){ 
		std::cout << "Counld not open file \n"; // mhnyma lathoys 
		addLogEntry("Counld not open file \n");
		return NULL;
	}

	Component * data = (Component *) malloc(sizeof(Component));
	unsigned int W = 0;
	unsigned int H = 0;
	int count = 0 ;
	unsigned int block_size=0;
	char * word  =(char*) malloc(sizeof(char));
	unsigned int cl=0;
	unsigned int times=0;

	while( count < 7) {  //oso yparxei string gia na diabasoyme 
		if(count == 0 ) {
			file.read(word,3);
			word[3] = '\0';
			if(strcmp(word,"CPI") != 0){
				std::cout << " Incorect  RLE format \n";
				addLogEntry("Incorect format \n");
				return NULL;
			}
			count++;
		}
		else if(count==1){
			unsigned short two;
			file.read((char*)word,1);
			two = *word;
			if(two!= 2)
			{
				std::cout << " Wrong edition (a=only 1 ) \n";
				addLogEntry(" Wrong edition (a=only 1 ) \n");
				return NULL;
			}
			count++;
		}
		else if ( count == 2 ){
			unsigned char hi , lo ;
			file.read(reinterpret_cast<char*>(&hi), 1);
			file.read(reinterpret_cast<char*>(&lo), 1);

			unsigned int endian = (lo << 8) | hi;
			if(endian != 258){
				std::cout << endian <<" Incorrect Endiannes\n";
				addLogEntry(" Incorrect Endiannes\n");
				break;
			}
			count ++ ; 
		}

		else if ( count == 3 ){
			file.read((char *) &W,2);
			count ++ ;
		}
		
		else if ( count == 4 ){
			file.read((char *) &H,2);
			count ++ ;
		}
			
		else if(count==5){
			file.read((char*)&block_size,2);
			count++;
		}


		
		else if(count == 6 ){
			data = (Component *) malloc(H*W*3*sizeof(Component));
			int c = 0;
			for(unsigned int j=0;j<W*H*3;j++){
				file.read((char*)&cl,1);
				file.read((char*)&times,1);
				for(unsigned int i=0 ; i<times ; i++){
					data[c+i]=(Component)cl;
					
				}
				c=c+times;
			 }
			count++;
		}
		
	}

	file.close();
	bool interleaved=false;
	Image * theImage = new Image(W,H,data,interleaved);
	return theImage;
}




