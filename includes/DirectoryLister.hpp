/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DirectoryLister.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 18:10:10 by dianarituay       #+#    #+#             */
/*   Updated: 2026/03/07 18:11:04 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORYLISTER_HPP
#define DIRECTORYLISTER_HPP

#include <string>
#include <vector>
#include <dirent.h>	 // Para opendir, readdir
#include <sys/stat.h> // Para stat, S_ISDIR
#include <sstream>	 // Para generar HTML

// Estructura para representar una entrada del directorio
struct DirEntry
{
	std::string name; // Nombre del archivo/carpeta
	bool isDirectory; // true si es carpeta, false si es archivo
	size_t size;		// Tamaño en bytes (0 para directorios)
};

class DirectoryLister
{
public:
	DirectoryLister();
	~DirectoryLister();

	// Función principal: genera HTML con el listado
	std::string generateHTML(const std::string &dirPath,
									 const std::string &uri);

private:
	// Leer contenido del directorio
	std::vector<DirEntry> listDirectory(const std::string &dirPath);

	// Verificar si es directorio
	bool isDirectory(const std::string &path);

	// Obtener tamaño de archivo
	size_t getFileSize(const std::string &path);

	// Formatear tamaño para mostrar (KB, MB, etc.)
	std::string formatSize(size_t bytes);
};

#endif