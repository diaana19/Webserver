#ifndef DIRECTORYLISTER_HPP
#define DIRECTORYLISTER_HPP

#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <sstream>

struct DirEntry
{
	std::string name;
	bool isDirectory;
	size_t size;
};

class DirectoryLister
{
public:
	DirectoryLister();
	~DirectoryLister();

	std::string generateHTML(const std::string &dirPath,
									 const std::string &uri);

private:
	std::vector<DirEntry> listDirectory(const std::string &dirPath);
	bool isDirectory(const std::string &path);
	size_t getFileSize(const std::string &path);
	std::string formatSize(size_t bytes);
};

#endif