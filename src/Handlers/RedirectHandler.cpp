/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectHandler.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dianarituay <dianarituay@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 10:43:26 by dianarituay       #+#    #+#             */
/*   Updated: 2026/02/26 10:43:27 by dianarituay      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RedirectHandler.hpp"
RedirectHandler::RedirectHandler(){};
RedirectHandler::~RedirectHandler(){};

bool RedirectHandler::hasRedirect(LocationConfig &location) {
	if (!location.redirect_target.empty()) {
		return (true);
	}
	return (false);
};

HttpResponse RedirectHandler::handle(LocationConfig &location) {
	HttpResponse httpResponse;
	httpResponse.headers["Location"] = location.redirect_target;
	if (location.redirect_code == -1)
		httpResponse.statusCode = 301;
	else
		httpResponse.statusCode = location.redirect_code;
	return (httpResponse);
};