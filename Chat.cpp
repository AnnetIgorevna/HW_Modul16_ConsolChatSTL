#include "Chat.h"
#include <iostream>
#include <string>

void Chat::showLoginMenu()
{
	_currentUser = -1;
	char operation;
	std::cout << "\n*** Welcome to Console Chat! ***" << std::endl;

	do
	{
		std::cout << " \n1 - Autorization \n2 - Registration \n0 - Exit" << std::endl;
		std::cin >> operation;

		switch (operation)
		{
		case '1':
			logIn();
			break;
		case '2':
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		case '0':
			_isChatWork = false;
			break;
		default:
			std::cout << "Wrong choice! Choose 0, 1 or 2" << std::endl;
			break;
		}
	} while (!_currentUser && _isChatWork);
}

void Chat::showUserMenu()
{
	std::cout << "\n*** You have entered the User menu ***" << std::endl;
	char operation;

	do
	{
		std::cout << "\n1 - Show all users \n2 - Show all messages"
			<< "\n3 - Add message to chat \n4 - Change password \n5 - Sign Out" << std::endl;
		std::cin >> operation;
		switch (operation)
		{
		case '1':
			showAllUsersLogin();
			break;
		case '2':
			showChat();
			break;
		case '3':
			addMessage();
			break;
		case '4':
			changePassword();
			break;
		case '5':
			_currentUser = -1;
			break;
		default:
			std::cout << "Wrong choice! Choose 1 - 5" << std::endl;
			break;
		}
	} while (_currentUser >= 0);

}

void Chat::logIn()
{
	std::string login, password;
	std::string operation;

	do {
		std::cout << "Enter your login: " << std::endl;
		std::cin >> login;
		_currentUser = getUserByLogin(login);
		if (_currentUser == -1)
		{
			std::cout << "The login entered is incorrect. To exit press 0, to continue press any key:" << std::endl;
			std::cin >> operation;
			if (operation == "0")
				return;
		}
		else
		{
			bool passwordOk = false;
			do
			{
				std::cout << "Enter your password: " << std::endl;
				std::cin >> password;
				for (size_t i = 0; i < _mem_size; i++)
				{
					int index = hash_func(login, i);
					std::unordered_map<int, User>::iterator it = _users.find(index);
					uint* PasHash = sha1(password.c_str(), password.length());
					if (it != _users.end() && !memcmp(it->second.getUserPassword(), PasHash, SHA1HASHLENGTHBYTES))
					{
						std::cout << "Hello " << it->second.getUserLogin() << "!" << std::endl;
						passwordOk = true;
						delete[] PasHash;
						break;
					}
					else
					{
						std::cout << "The password entered is incorrect. To exit press 0, to continue press any key:" << std::endl;
						std::cin >> operation;
						if (operation == "0")
						{
							_currentUser = -1;
							return;
						}
						break;
					}
				}
			} while (!passwordOk);
		}
	} while (!(_currentUser >= 0));
}

void Chat::signUp()
{
	std::string login, password;
	bool passwordOk = false;

	std::cout << "Enter your login: " << std::endl;
	std::cin >> login;
	if (getUserByLogin(login) != -1 || login == "all")
	{
		throw UserLoginExp();
	}
	while (!(passwordOk))
	{
		std::cout << "Enter your password: " << std::endl;
		std::cin >> password;
		if (password.length() < 4 || password.length() > 8)
			throw UserPasswordExp();
		else
		{
			passwordOk = true;
			uint* _pass_sha1_hash = sha1(password.c_str(), password.length());
			addUser(login, _pass_sha1_hash);
		}
	}
}

void Chat::addUser(std::string login, uint* password)
{
	if (_data_count >= _mem_size)
		_mem_size *= 2;

	int index = -1;
	int i = 0;
	for (; i < _mem_size; i++) {
		index = hash_func(login, i);
		std::unordered_map<int, User>::iterator it = _users.find(index);
		if (it == _users.end() || !_users.size() || it->second.getUserStatus() == 0)
			break;
	}
	if (i >= _mem_size)
	{
		_mem_size *= 2;
		addUser(login, password);
	}
	else
	{
		User user = User(login, password);
		_users.emplace(index, user);
		std::unordered_map<int, User>::iterator it = _users.find(index);
		_currentUser = it->first;
		_data_count++;
	}
}

int Chat::hash_func(const std::string& login, int offset) {
	long sum = 0;
	for (int i = 0; i < login.length(); i++)
	{
		sum += login[i];
	}
	return (hf_multiply(sum) + offset * offset) % _mem_size;
}

int Chat::hf_multiply(int val) {
	const double A = 0.7;
	return int(_mem_size * (A * val - int(A * val)));
}

int Chat::getUserByLogin(const std::string& login) const
{
	for (auto& user : _users)
	{
		if (login == user.second.getUserLogin())
		{
		return user.first;
		}
	}
	return -1;
}

void Chat::showAllUsersLogin()
{
		std::cout << "\n*** All users in this Chat ***" << std::endl;
	
	for (auto& user : _users)
	{
		if (_currentUser == user.first)
		{
			std::cout << "Me";
		}
		else
		{
			std::cout << user.second.getUserLogin();
		}
		std::cout << std::endl;
	}
	std::cout << "******************************" << std::endl;
}

void Chat::showChat()
{
	std::string from, to;

	if (!(_messages.size()))
	{
		std::cout << "It's no messages in this chat!" << std::endl;
		return;
	}
	std::cout << "\n*** This is all messages in this chat ***" << std::endl;

	for (auto& message : _messages)
	{
		std::unordered_map<int, User>::iterator it = _users.find(getCurrentUser());
		if (it->second.getUserLogin() == message.getFrom() || it->second.getUserLogin() == message.getTo() || message.getTo() == "all")
		{
			std::unordered_map<int, User>::iterator it2 = _users.find(getUserByLogin(message.getFrom()));
			from = (it->second.getUserLogin() == message.getFrom()) ? "Me" : it2->second.getUserLogin();
			if (message.getTo() == "all")
			{
				to = "all";
			}
			else
			{
				std::unordered_map<int, User>::iterator it3 = _users.find(getUserByLogin(message.getTo()));
				to = (it->second.getUserLogin() == message.getTo()) ? "Me" : it3->second.getUserLogin();
			}
			std::cout << "From: " << from << "\nTo: " << to << std::endl;
			std::cout << "Text: " << message.getText() << std::endl << std::endl;
		}
	}
	std::cout << "*** End of chat ***" << std::endl;
}

void Chat::addMessage()
{
	std::string to, text;

	std::cout << "\nTo (login of user or all): " << std::endl;
	std::cin >> to;
	std::cout << "Enter the text: " << std::endl;
	std::cin.ignore();
	std::getline(std::cin, text);

	if (!(to == "all" || getUserByLogin(to) != -1))
	{
		std::cout << "Error send message: can't find user with name " << to << std::endl;
		return;
	}
	std::unordered_map<int, User>::iterator it = _users.find(getCurrentUser());
	if (to == "all")
	{
		_messages.push_back(Message{ it->second.getUserLogin(), "all", text});
	}
	else
	{
		std::unordered_map<int, User>::iterator it2 = _users.find(getUserByLogin(to));
		_messages.push_back(Message{ it->second.getUserLogin(), it2->second.getUserLogin(), text });
	}
}

void Chat::changePassword()
{
	std::string password;
	std::cout << "Enter new password: " << std::endl;
	std::cin >> password;
	if (password.length() < 4 || password.length() > 8)
		throw UserPasswordExp();
	else
	{
		for (auto& user : _users)
		{
			uint* _pass_sha1_hash = sha1(password.c_str(), password.length());
			std::unordered_map<int, User>::iterator it = _users.find(getCurrentUser());
			if (it->second.getUserLogin() == user.second.getUserLogin())
				user.second.setUserPassword(_pass_sha1_hash);
		}
	}
}
