#include "LicensingIO.h"
#include <iostream>
#include <fstream>
#include "LicensingLog.h"
#ifndef _WIN32
#include <sys/stat.h>
#endif
FileInputBuffer::FileInputBuffer(const char *fileName)
	: m_fileName(fileName)
{
  
}

bool FileInputBuffer::getBytes(uint8_t **outputBytes, size_t &maxSize)
{
	maxSize = 0;

	if (!outputBytes)
	{
		log(Error, "FileInputBuffer::getBytes(): invalid input parameters");
		return false;
	}
	struct stat statStr;
	if (0 != stat(m_fileName.c_str(), &statStr))
	{
		log(Error, "FileInputBuffer::getBytes(): failed to find file");
		log(Error, m_fileName.c_str());
		return false;
	}

	std::ifstream lowLevelInput(m_fileName.c_str(), std::ios_base::binary);
	if (!lowLevelInput.is_open())
	{
		log(Error, "FileInputBuffer::getBytes(): file is not open");
		return false;
	}
	if (statStr.st_size <= 0) 
	{
		log(Error, "FileInputBuffer::getBytes(): file is empty");
		return false;
	}
	*outputBytes = new uint8_t[statStr.st_size];

	lowLevelInput.read((char *)*outputBytes, statStr.st_size);

	maxSize = lowLevelInput.gcount();

	lowLevelInput.close();
	return true;
}

FileOutputBuffer::FileOutputBuffer(const char *fileName)
	: m_fileName(fileName)
{
    //m_fileName = std::regex_replace(m_fileName, std::regex(" "), "\\ ");
}

bool FileOutputBuffer::putBytes(uint8_t *inputBytes, size_t size)
{
	if (!inputBytes || !size)
	{
		log(Error, "FileOutputBuffer::putBytes(): invalid input parameters");
		return 0;
	}
	std::ofstream lowLevelOutput(m_fileName);
	if (!lowLevelOutput.is_open())
	{
		log(Error, "FileInputBuffer::putBytes(): file is not open");
		return 0;
	}
	lowLevelOutput.write((char *)inputBytes, size);
	size_t written = lowLevelOutput.tellp();
	lowLevelOutput.close();
#ifndef _WIN32
    int chmodRv = chmod(m_fileName.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (chmodRv < 0)
    {
        log(Error,"failed to chmod the license file");
    }
#endif
	return written == size;
}
