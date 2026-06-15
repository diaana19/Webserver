RED		= \033[0;31m
GREEN		= \033[0;32m
YELLOW	= \033[0;33m
BLUE		= \033[0;34m
CYAN		= \033[0;36m
RESET		= \033[0m

NAME	= webserver
CC		= c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 -Iincludes  -MMD -MP

SRC	= \
		src/Networking/Client.cpp \
		src/Networking/ClientManager.cpp \
		src/Networking/Server.cpp \
		src/Networking/EventLoop_utils.cpp \
		src/Networking/EventLoop.cpp \
		src/Networking/EventLoop_read.cpp \
		src/Networking/EventLoop_constructRes.cpp \
		src/Networking/EventLoop_write.cpp \
		src/Networking/StartServer.cpp \
		src/ConfigParsing/Config.cpp \
		src/ConfigParsing/Token.cpp \
		src/ConfigParsing/ReadFile.cpp \
		src/ConfigParsing/LocationParsing.cpp \
		src/ConfigParsing/ServerParsing.cpp \
		src/ConfigParsing/ConfigParser.cpp \
		src/ConfigParsing/ParseConfig.cpp \
		src/HttpParsing/HeadersParsing.cpp \
		src/HttpParsing/BodyParser.cpp \
		src/HttpParsing/RequestLine.cpp \
		src/HttpParsing/RequestState.cpp \
		src/HttpResponse/HeadersBuilder.cpp \
		src/HttpResponse/BodyBuilder.cpp \
		src/HttpResponse/StatusBuilder.cpp \
		src/HttpResponse/ErrorHandler.cpp \
		src/Handlers/PostHandler.cpp \
		src/Handlers/GetHandler.cpp \
		src/Handlers/RedirectHandler.cpp \
		src/Handlers/DeleteHandler.cpp \
		src/Path/LocationPathTransform.cpp \
		src/CGI/CGIPipes.cpp \
		src/CGI/CGIExecutor.cpp\
		src/CGI/CGIEnvironment.cpp\
		src/File/FileServer.cpp \
		src/File/DirectoryLister.cpp \
		src/Cookies/CookieParser.cpp\
		src/Cookies/CookieSender.cpp\
		src/Session/SessionManager.cpp \
		main.cpp 

OBJDIR	= obj
OBJS	= $(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))
DEPENDENCE = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✅ $(NAME) compiled!$(RESET)"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if [ -d "$(OBJDIR)" ]; then \
		echo "$(RED)Cleaning objects...$(RESET)"; \
		rm -rf $(OBJDIR); \
		echo "$(GREEN)✅ Clean done!$(RESET)"; \
	else \
		echo "$(YELLOW)Nothing to clean.$(RESET)"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		echo "$(RED)Removing $(NAME)...$(RESET)"; \
		rm -f $(NAME); \
		echo "$(GREEN)✅ Full clean done!$(RESET)"; \
	else \
		echo "$(YELLOW)Nothing to remove.$(RESET)"; \
	fi

re: fclean all

-include $(DEPENDENCE)

.PHONY: all clean fclean re
