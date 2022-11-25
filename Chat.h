#pragma once

#include <vector>
#include <unordered_map>
#include <exception>
#include <memory>

#include "Message.h"

/*���������-����������*/

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
	void start() { _isChatWork = true; }; // ������ ������ ����, ��������� _isChatWork � true
	bool isChatWork() const { return _isChatWork; } // ���������� ������� ��������� ���� ��������/�� ��������
	int getCurrentUser() const { return _currentUser; } // ���������� ���� �������� ������������
	void showLoginMenu(); // ���������� �������������� ����
	void showUserMenu(); // ���������� ���� �������� ������������

private:
	bool _isChatWork = false; // ����������, ���������� �� ����������� ������ ���� (� ������ ������ ���� ��������� � true)
	std::unordered_map<int,User> _users; // ���������, ���������� ���� ������������������ �������������
	std::vector<Message> _messages; // ������ - ������, ���������� ��� ������������ � ��� ���������
	int _currentUser = -1; // ��������� �� �������� ������������
	int _data_count = 0; // ���������� ������������������ �������������
	int _mem_size = 8;

	void logIn(); // ���� ����������� ������������
	void signUp(); // ���� ����������� ������ ������������
	void addUser(std::string login, uint* password); // ���������� ������������ � unordered_map
	int hash_func(const std::string& login, int offset); // ���-�������, ����������� ���� ��� unordered_map
	int hf_multiply(int val);
	void showAllUsersLogin(); // �������� ���� �������������, ���������������� � ����
	void showChat(); // �������� ��� ��������� � ����
	void addMessage(); // �������� ��������� ���� / ����������� ������������
	void changePassword(); // ��������� ������ �������� ������������

	int getUserByLogin(const std::string& login) const; // ����� ������������ �� ������, ���������� ���� � unordered_map
};
