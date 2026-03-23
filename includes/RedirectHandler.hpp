/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectHandler.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:42:23 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:42:25 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
# ifndef REDIRECTHANDLER_HPP
# define REDIRECTHANDLER_HPP

# include <iostream>
# include <string>
# include "Config.hpp"
# include "HttpResponse.hpp"
# include <fstream>
# include <sstream>
# include <cstdlib>

class RedirectHandler
{
	public:
		RedirectHandler();
		~RedirectHandler();
		bool hasRedirect(LocationConfig &location);
		HttpResponse handle(LocationConfig &location);
	// ??
};


#endif