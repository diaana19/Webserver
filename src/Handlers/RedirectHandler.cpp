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