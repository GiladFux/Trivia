#include "LoginManager.h"
#include <exception>
LoginManager::LoginManager(IDataBase* db) :m_db(db)
{
}

bool LoginManager::signup(const std::string& username, const std::string& password, const std::string& email, SOCKET sock)
{
	//checking if such a user with the same username already exists
	if (m_db->doesUserExists(username))
	{
		throw UserAlreadyExistsException();
		return false;
	}
	m_db->addNewUser(username, password, email);
	LoggedUser* user = new LoggedUser(username, sock);
	m_loggedUsers.push_back(*user);
	return true;
}

bool LoginManager::login(const std::string& username, const std::string& password, SOCKET sock)
{
	LoggedUser* user = new LoggedUser(username, sock);
	json response;
	//checking if user already logged in 
	auto index = std::find(m_loggedUsers.begin(), m_loggedUsers.end(), *user);
	if (index != m_loggedUsers.end())
	{
		throw AlreadyLoggedInException();
		return false;
	}
	//checking if such user exists in the data base 
	if (!m_db->doesUserExists(username))
	{
		throw WrongDetailsException();
		return false;

	}
	//checking if the password is correct 
	if (!m_db->doesPasswordMatch(username, password))
	{
		throw WrongDetailsException();
		return false;
	}
	//if he passed all tests adding him to m_loggedUsers
	m_loggedUsers.push_back(*user);
	return true;
}


bool LoginManager::logout(const LoggedUser& user)
{
	//checking if the user is logged in 
	json response;

	auto index = std::find(m_loggedUsers.begin(), m_loggedUsers.end(), user);
	if (index != m_loggedUsers.end())
	{
		m_loggedUsers.erase(index);
		return true;
	}
	return false;
	
}

LoggedUser& LoginManager::getLoggedUser(const std::string& username) 
{
	for (auto& user : m_loggedUsers) 
	{
		if (user.getName() == username) 
		{
			return user;
		}
	}
}

LoggedUser LoginManager::getUserBySocket(SOCKET userSock)
{
	for (auto user : m_loggedUsers)
	{
		if (user.getSocket() == userSock)
		{
			return user;
			break;
		}
		
	}
	throw userSock;
}


