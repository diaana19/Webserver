#include "DirectoryLister.hpp"
#include <iostream>
#include <algorithm>

DirectoryLister::DirectoryLister()
{
}

DirectoryLister::~DirectoryLister()
{
}


bool DirectoryLister::isDirectory(const std::string &path)
{
	struct stat buffer;

	
	if (stat(path.c_str(), &buffer) != 0)
		return false;

	
	return (S_ISDIR(buffer.st_mode));
}


size_t DirectoryLister::getFileSize(const std::string &path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		return 0;

	
	return (buffer.st_size);
}


std::string DirectoryLister::formatSize(size_t bytes)
{
	std::ostringstream oss;

	if (bytes < 1024)
	{
		oss << bytes << " B";
	}
	else if (bytes < 1024 * 1024)
	{
		oss << (bytes / 1024) << " KB";
	}
	else if (bytes < 1024 * 1024 * 1024)
	{
		oss << (bytes / (1024 * 1024)) << " MB";
	}
	else
	{
		oss << (bytes / (1024 * 1024 * 1024)) << " GB";
	}

	return (oss.str());
}

static bool compareDirEntries(const DirEntry& a, const DirEntry& b) 
{
    
    if (a.isDirectory != b.isDirectory)
        return a.isDirectory > b.isDirectory;  
    
    
    return a.name < b.name;
}

std::vector<DirEntry> DirectoryLister::listDirectory(const std::string &dirPath)
{
	std::vector<DirEntry> entries; 

	DIR *dir = opendir(dirPath.c_str());
	if (!dir)
	{
		std::cerr << "[DirectoryLister] Failed to open: " << dirPath << std::endl;
		return entries;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;
		if (name == "." || name == "..")
			continue;
		std::string fullPath = dirPath;
		if (!fullPath.empty() && fullPath[fullPath.length() - 1] != '/')
			fullPath += "/";
		fullPath += name;
		DirEntry dirEntry;
		dirEntry.name = name;
		dirEntry.isDirectory = isDirectory(fullPath);
		dirEntry.size = dirEntry.isDirectory ? 0 : getFileSize(fullPath);
		entries.push_back(dirEntry);
	}

	closedir(dir);
	std::sort(entries.begin(), entries.end(), compareDirEntries);
	return (entries);
}


std::string DirectoryLister::generateHTML(const std::string &dirPath,
														const std::string &uri)
{
	std::ostringstream html;
	std::vector<DirEntry> entries = listDirectory(dirPath);

	html << "<!DOCTYPE html>\n";
	html << "<html>\n";
	html << "<head>\n";
	html << "    <meta charset=\"UTF-8\">\n";
	html << "    <title>Index of " << uri << "</title>\n";
	html << "    <style>\n";
	html << "        body {\n";
	html << "            font-family: Arial, sans-serif;\n";
	html << "            margin: 40px;\n";
	html << "            background: #f5f5f5;\n";
	html << "        }\n";
	html << "        h1 {\n";
	html << "            color: #333;\n";
	html << "            border-bottom: 2px solid #4CAF50;\n";
	html << "            padding-bottom: 10px;\n";
	html << "        }\n";
	html << "        table {\n";
	html << "            width: 100%;\n";
	html << "            background: white;\n";
	html << "            border-collapse: collapse;\n";
	html << "            box-shadow: 0 2px 4px rgba(0,0,0,0.1);\n";
	html << "        }\n";
	html << "        th {\n";
	html << "            background: #4CAF50;\n";
	html << "            color: white;\n";
	html << "            padding: 12px;\n";
	html << "            text-align: left;\n";
	html << "        }\n";
	html << "        td {\n";
	html << "            padding: 10px;\n";
	html << "            border-bottom: 1px solid #eee;\n";
	html << "        }\n";
	html << "        tr:hover {\n";
	html << "            background: #f9f9f9;\n";
	html << "        }\n";
	html << "        a {\n";
	html << "            text-decoration: none;\n";
	html << "            color: #0066cc;\n";
	html << "        }\n";
	html << "        a:hover {\n";
	html << "            text-decoration: underline;\n";
	html << "        }\n";
	html << "        .dir { font-weight: bold; }\n";
	html << "        .size { color: #666; }\n";
	html << "    </style>\n";
	html << "</head>\n";
	html << "<body>\n";

	html << "    <h1>📁 Index of " << uri << "</h1>\n";

	html << "    <table>\n";
	html << "        <tr>\n";
	html << "            <th>Name</th>\n";
	html << "            <th>Size</th>\n";
	html << "        </tr>\n";

	if (uri != "/")
	{
		html << "        <tr>\n";
		html << "            <td><a href=\"../\" class=\"dir\">📁 ../</a></td>\n";
		html << "            <td class=\"size\">-</td>\n";
		html << "        </tr>\n";
	}

	for (size_t i = 0; i < entries.size(); i++)
	{
		const DirEntry &entry = entries[i];

		std::string href = uri;
		if (!href.empty() && href[href.length() - 1] != '/')
			href += "/";
		href += entry.name;

		if (entry.isDirectory)
			href += "/";

		html << "        <tr>\n";
		html << "            <td>";

		if (entry.isDirectory)
		{
			html << "<a href=\"" << href << "\" class=\"dir\">📁 "
				  << entry.name << "/</a>";
		}
		else
		{
			html << "<a href=\"" << href << "\">📄 "
				  << entry.name << "</a>";
		}

		html << "</td>\n";
		html << "            <td class=\"size\">";

		if (entry.isDirectory)
		{
			html << "-";
		}
		else
		{
			html << formatSize(entry.size);
		}

		html << "</td>\n";
		html << "        </tr>\n";
	}

	html << "    </table>\n";
	
	html << "    <hr>\n";
	html << "    <p><em>webserv/1.0</em></p>\n";
	html << "</body>\n";
	html << "</html>\n";

	return (html.str());
}