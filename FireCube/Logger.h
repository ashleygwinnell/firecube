#ifndef LOGGER_H
#define LOGGER_H

namespace Logger
{
	void FIRECUBE_API Init(string filename);
	void FIRECUBE_API Write(string str);	
	void FIRECUBE_API Close();
}

#endif