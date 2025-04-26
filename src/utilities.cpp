#include "utilities.h"

#include <fstream>

//--------------------------------------------------------------------

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if(std::string::npos == first){
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

//--------------------------------------------------------------------

void mkfile(const char* filePath)
{
	std::ifstream file;
	file.open(filePath);
	if(!file.is_open()){
		std::ofstream outfile(filePath, std::ios::out);
	}
	file.close();
}

//--------------------------------------------------------------------

Instruction::Instruction(const std::string& str)
:m_moduloID(-1),
m_direction(-1),
m_short(false)
{
	int part=0;
	size_t t=0;
	size_t p=0;
	const int ds=1;// size of separator: " "
	while(p!=std::string::npos){
		p=str.find(" ", t);
		try{
			if(p==std::string::npos){
				m_short=static_cast<bool>(std::stoi(str.substr(t)));
				break;
			}
			if(part==0){
				m_moduloID=std::stoi(str.substr(t, p-t));
				part++;
			}
			else if(part==1){
				m_direction=std::stoi(str.substr(t, p-t));
				part++;
			}
		}
		catch(...){
			break;
		}
		t=p+ds;
	}
}

//====================================================================

std::string resourcePath(const char* fileName)
{
	std::string filePath="";
	 
	#ifdef DEBUG
	filePath.append(DEBUG_DIR); // define in CMakeLists
	filePath.append("/utilities/");
	#else
	if(getenv("APPDIR")){
		filePath.append(getenv("APPDIR"));
		filePath.append("/usr/share/utilities/");
	}
	#endif

	filePath.append(fileName);

	return filePath;
}

//====================================================================
