#ifndef __LICENSING_IO_H__
#define __LICENSING_IO_H__
#include <string>

class InputBuffer
{
public:
	virtual ~InputBuffer() {}
	virtual bool getBytes(uint8_t **outputBytes, size_t &maxSize) = 0;
};

class OutputBuffer
{
public:
	virtual ~OutputBuffer() {}
	virtual bool putBytes(uint8_t *inputBytes, size_t size) = 0;
};

class FileInputBuffer
	: public InputBuffer
{
public:
	FileInputBuffer(const char *fileName);

	virtual bool getBytes(uint8_t **outputBytes, size_t &maxSize);
private:
	std::string m_fileName;
};

class FileOutputBuffer
	: public OutputBuffer
{
public:
	FileOutputBuffer(const char *fileName);
	virtual bool putBytes(uint8_t *inputBytes, size_t size);
private:
	std::string m_fileName;

};
#endif//__LICENSING_IO_H__
