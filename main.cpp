#include "Chat.h"

int main()
{
	Chat chat;
	chat.start();
	while (chat.isChatWork())
	{
		chat.showLoginMenu();
		while (chat.getCurrentUser() >= 0)
		{
			chat.showUserMenu();
		}
	}
	return 0;
}