#pragma once

#include <vector>
#include <unordered_map>
#include <exception>
#include <memory>

#include "Message.h"

/*Структуры-исключения*/

struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "error: user login is bisy"; }
};
struct UserPasswordExp : public std::exception
{
	const char* what() const noexcept override { return "error: the password length must be from 4 to 8 digits or letters"; }
};

class Chat
{
public:
	void start() { _isChatWork = true; }; // начало работы чата, установка _isChatWork в true
	bool isChatWork() const { return _isChatWork; } // возвращает текущее состояние чата работает/не работает
	int getCurrentUser() const { return _currentUser; } // возвращает ключ текущего пользователя
	void showLoginMenu(); // показывает приветственное меню
	void showUserMenu(); // показывает меню текущего пользователя

private:
	bool _isChatWork = false; // переменная, отвечающая за прекращение работы чата (в начале работы чата переходит в true)
	std::unordered_map<int,User> _users; // контейнер, содержащий всех зарегистрированных пользователей
	std::vector<Message> _messages; // вектор - массив, содержащий все отправленные в чат сообщения
	int _currentUser = -1; // указатель на текущего пользователя
	int _data_count = 0; // количество зарегистрированных пользователей
	int _mem_size = 8;

	void logIn(); // меню авторизации пользователя
	void signUp(); // меню регистрации нового пользователя
	void addUser(std::string login, uint* password); // добавление пользователя в unordered_map
	int hash_func(const std::string& login, int offset); // хеш-функция, вычисляющая ключ для unordered_map
	int hf_multiply(int val);
	void showAllUsersLogin(); // показать всех пользователей, зарегистрованных в чате
	void showChat(); // показать все сообщения в чате
	void addMessage(); // написать сообщение всем / конкретному пользователю
	void changePassword(); // изменение пароля текущего пользователя

	int getUserByLogin(const std::string& login) const; // найти пользователя по логину, возвращает ключ в unordered_map
};
