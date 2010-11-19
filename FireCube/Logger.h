#ifndef LOGGER_H
#define LOGGER_H

namespace FireCube
{
namespace Logger
{
	void FIRECUBE_API Init(std::string filename);
	void FIRECUBE_API Write(std::string str);	
	void FIRECUBE_API Close();
}
}
#endif