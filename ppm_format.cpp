#include "ppm_format.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace imaging;
enum readState { MODE=0 , WIDTH , HEIGHT, LIMIT ,PIXEL} ; //theloume na diabasoume 5 pragmata kai gia na ta dimiourgisoyme ftiaxnoume katastaseis

void PPMImageWriter::write(std::string filename,const Image & src){
	std::ofstream file;
	
	std::string name;
	
	if (isSuffix(filename,"cpi"))
		replaceSuffix(filename,"ppm");
		
	
	file.open(filename, std::ios::binary| std::ios::out |std::ios::trunc );

	if(!file.is_open()){ 
		std::cout << "Counld not open file \n"; // mhnyma lathoys 
		addLogEntry("Counld not open file \n");
	}

	/*se periptwsh pou gia kapoio logo den kataferei na anoixei to arxeio 
	file==NULL opote h parakatw sunthikh den tha isxyei */

	if (file != NULL ){
		
		unsigned short w=src.getWidth();
		unsigned short h=src.getHeight();

		//xrisimopoioume thn write methodo gia na valoume kena metaxi twn stoixeiwn ths kefalidas
		file <<"P6";
		file.write(" ",1);
		file << src.getWidth();
		file.write(" ",1);
		file << src.getHeight();
		file.write(" ",1);
		file << "255";
		file.write(" ",1);
		

		Image *myImage = new Image(src.getWidth(), src.getHeight(), src.getRawDataPtr(),true);

		if(!src.isInterleaved())
			myImage->convertToInterleavedImage();

		for (int i =0 ; i<=3*h*w;i++){  
			file.write((char*)myImage->getRawDataPtr()+i,1);
		}
		addLogEntry("Succesfully wrote image");
	}
	else {
		std::cout << "Cannot open file :";
		std:: cout << filename ;
		std:: cout << "\n It might be open from an other application\n";
		addLogEntry("Counld not open file , It might be open from an other application \n");
	}
	
	file.close();
}


Image * PPMImageReader::read(std::string filename){
	std::ifstream file;
	file.open(filename, std::ios::binary); // anoigoyme to arxeio se binary morfi
	
	if(!file.is_open()){ 
		addLogEntry("Counld not open file \n"); // mhnyma lathoys 
		std::cout << "Counld not open file \n";
		return NULL;
	}
	
	std::string mode;
	Component * data = (Component *) malloc(sizeof(Component));
	int W = 0;
	int H = 0;
	int limit = 0;
	char comment='#';
	
	std::string word;
	readState rst = MODE ;
	while(file >> word){  //oso yparxei string gia na diabasoyme 
		
		if(word[0] == comment){ //an h leji pou diavazoume xekinaei me # tote pareleipse thn kai phda sthn epomenh grammh
			std::getline(file,word); //getline = diavazei mia olokliri grammi kai paei stthn apo katw		
		}
		else{
			switch (rst) { // periptvseis : 
			case MODE:		
				mode=word;
				if(mode!="P6"){
					addLogEntry("The image is not P6 format");
					std:: cout << "ERROR: Application supports only P6 format \n" ;
					return NULL;
				}
				rst = WIDTH;
				break;
			case WIDTH:
				W = std::atoi (word.c_str());
				rst = HEIGHT;
				break;
			case HEIGHT:
				H = std::atoi (word.c_str());
				rst = LIMIT;
				break;
			case LIMIT:
				limit = std::atoi(word.c_str());
				if(limit>255){
					addLogEntry("The image is out of color range");
					std:: cout << "ERROR: Application supports only color range 0-255 \n" ;
					return NULL;
				}
				rst = PIXEL;
				std::getline(file,word);
				break;
			}
			
			if(rst == PIXEL){
				addLogEntry("Succefully read Image data");
				data = (Component*)calloc(H*W*3,sizeof(Component));
				for( int i=0; i<H*W*3; i++){
					Component cl;
					file.read((char *)&cl,1);
					data[i] = cl;
				}
			}
		}
	}
	file.close();
	Image * theImage = new Image(W,H,data,true);
	return theImage;
}

