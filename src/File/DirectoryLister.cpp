/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryLister.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 18:10:26 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/07 18:26:57 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "DirectoryLister.hpp"
#include <iostream>
#include <algorithm>

DirectoryLister::DirectoryLister()
{
}

DirectoryLister::~DirectoryLister()
{
}

//verifica si es directory
bool DirectoryLister::isDirectory(const std::string &path)
{
	struct stat buffer;

	// stat() obtiene información del archivo
	if (stat(path.c_str(), &buffer) != 0)
		return false;

	// S_ISDIR verifica si es directorio
	return (S_ISDIR(buffer.st_mode));
}

//obtiene size del file
size_t DirectoryLister::getFileSize(const std::string &path)
{
	struct stat buffer;

	if (stat(path.c_str(), &buffer) != 0)
		return 0;

	// st_size contiene el tamaño en bytes
	return (buffer.st_size);
}

// formatea el size (bytes → KB, MB, etc.)
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


// Función de comparación para ordenar
// Directorios primero, luego alfabético
static bool compareDirEntries(const DirEntry& a, const DirEntry& b) 
{
    // Si uno es directorio y el otro no
    if (a.isDirectory != b.isDirectory)
        return a.isDirectory > b.isDirectory;  // Dirs primero
    
    // Si ambos son del mismo tipo, ordenar alfabéticamente
    return a.name < b.name;
}

// lee contenido del directory
std::vector<DirEntry> DirectoryLister::listDirectory(const std::string &dirPath)
{
	std::vector<DirEntry> entries; //files + directory

	// Abrir directorio
	DIR *dir = opendir(dirPath.c_str());
	if (!dir)
	{
		std::cerr << "[DirectoryLister] Failed to open: " << dirPath << std::endl;
		return entries;
	}

	struct dirent *entry;

	// Leer cada entrada del directorio
	while ((entry = readdir(dir)) != NULL)
	{
		std::string name = entry->d_name;

		// Ignorar . y ..
		if (name == "." || name == "..")
			continue;

		// Construir path completo
		std::string fullPath = dirPath;
		if (!fullPath.empty() && fullPath[fullPath.length() - 1] != '/')
			fullPath += "/";
		fullPath += name;

		// Crear entrada
		DirEntry dirEntry;
		dirEntry.name = name;
		dirEntry.isDirectory = isDirectory(fullPath);
		dirEntry.size = dirEntry.isDirectory ? 0 : getFileSize(fullPath);

		entries.push_back(dirEntry);
	}

	closedir(dir);

	// Ordenar: directorios primero, luego alfabético
	std::sort(entries.begin(), entries.end(), compareDirEntries);

	return (entries);
}

//generar html con el listado
std::string DirectoryLister::generateHTML(const std::string &dirPath,
														const std::string &uri)
{
	std::ostringstream html;

	// Obtener lista de archivos/carpetas
	std::vector<DirEntry> entries = listDirectory(dirPath);

	//header html
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

	//titulo
	html << "    <h1>📁 Index of " << uri << "</h1>\n";

	//tabla
	html << "    <table>\n";
	html << "        <tr>\n";
	html << "            <th>Name</th>\n";
	html << "            <th>Size</th>\n";
	html << "        </tr>\n";

	//link al directory padre
	if (uri != "/")
	{
		html << "        <tr>\n";
		html << "            <td><a href=\"../\" class=\"dir\">📁 ../</a></td>\n";
		html << "            <td class=\"size\">-</td>\n";
		html << "        </tr>\n";
	}

	//listar archivos y carpetas
	for (size_t i = 0; i < entries.size(); i++)
	{
		const DirEntry &entry = entries[i];

		// Construir href (enlace)
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

	//footer
	html << "    <hr>\n";
	html << "    <p><em>webserv/1.0</em></p>\n";
	html << "</body>\n";
	html << "</html>\n";

	return (html.str());
}