#pragma once

class LoginManager
{
public:
	static LoginManager* GetInstance();

	void Login();
	void Logout();
};
