#ifndef __LICENSING_LOG_H__
#define __LICENSING_LOG_H__

#ifdef __APPLE__
#ifdef LICENSE_IOS
#define LICENSE_LOG 0

#else

#define LICENSE_LOG 1
#endif//TARGET_OS_MAC

#else
#ifdef __ANDROID__
#define LICENSE_LOG 2
#else//windows
#define LICENSE_LOG 1

#endif//__ANDROID__
#endif//__APPLE__

#if (LICENSE_LOG==2)//Android
#include <android/log.h>
enum LogLevel
{
	Debug = ANDROID_LOG_DEBUG,
	Warn = ANDROID_LOG_INFO,
	Error = ANDROID_LOG_ERROR
};
void log(int level, const char *message);

#elif (LICENSE_LOG==0)//iOS

enum LogLevel
{
    Debug = 0,
    Warn = 1,
    Error = 2
};

void log(int level, const char *message);

#else//win,mac os x

#include <fstream>
#include <cstring>

enum LogLevel
{
    Debug = 0,
    Warn = 1,
    Error = 2,
    None = 4
};

class LicenseLogger
{
public:
	static LicenseLogger *instance();
    static void shutdown();
	void readLogSettings(const char *file);
    void configureLogSettings(const char *file, size_t max_size, int log_level, size_t max_files_count);
	void logMessage(int level, const char *fileName, int line, const char *message);
	~LicenseLogger();
private:
	LicenseLogger();
    
    void openLogFile(const char *fileName);
    
	static LicenseLogger *s_instance;
	unsigned long m_maxLogFileSize;
	int m_logLevel;
	int m_maxOldFilesCount;
	int m_WrittenSoFar;
	std::ofstream m_logStream;
	std::string m_logFolder;
	std::string m_logFileName;
};
#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define log(level,message) LicenseLogger::instance()->logMessage(level, __FILENAME__, __LINE__, message)

#endif //__ANDROID__





#endif//__LICENSING_LOG_H__
