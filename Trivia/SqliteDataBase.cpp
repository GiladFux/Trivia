#include "SqliteDataBase.h"
#include <regex>
#define DBName "UsersDB.sqlite"


SqliteDatabase::SqliteDatabase()
{
    if (!open())
        CreateDB();

}

bool SqliteDatabase::doesUserExists(const std::string& username)
{
    int count = 0;
    executeOperation("SELECT COUNT(*) FROM users WHERE USERNAME ='" + username + "';", countCallBack, &count);
    return count > 0;

}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password)
{
    int count = 0;
    executeOperation("SELECT COUNT(*) FROM users WHERE USERNAME ='" + username + "' AND PASSWORD ='" + password + "';", countCallBack, &count);
    return count > 0;
}

void SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email)
{
    executeOperation("INSERT INTO users (USERNAME, PASSWORD, EMAIL) VALUES ('" + username + "', '" + password + "','" + email + "');");
    executeOperation("INSERT INTO statistics VALUES ('" + username + "', " + std::to_string(0) + ", " + std::to_string(0) + ", " + std::to_string(0) + ", " + std::to_string(0) + ");");

}

std::list<Question> SqliteDatabase::getQuestions(const int amount)
{
    std::list<Question> questions;
    std::string sqlStatement = "SELECT QUESTION, CORRECT_ANSWER, INCORRECT_ANSWER_1, INCORRECT_ANSWER_2, INCORRECT_ANSWER_3 FROM Questions LIMIT " + std::to_string(amount) + ';';
    executeOperation(sqlStatement, questionCallBack, &questions);
    return questions;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& playerName) 
{
    float avgTime = 0;
    executeOperation("SELECT AVG_TIME FROM STATISTICS WHERE USERNAME = '" + playerName + "';", countCallBack, &avgTime);
    return avgTime;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& playerName)
{
    float amountOfCorrect = 0;
    executeOperation("SELECT CORRECT_ANS FROM STATISTICS WHERE USERNAME = '" + playerName + "';", countCallBack, &amountOfCorrect);
    return (int)amountOfCorrect;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& playerName)
{
    float totalAns = 0;
    executeOperation("SELECT TOTAL_ANS FROM STATISTICS WHERE USERNAME = '" + playerName + "';", countCallBack, &totalAns);
    return (int)totalAns;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& playerName)
{
    float totalGames = 0;
    executeOperation("SELECT GAMES_AMOUNT FROM STATISTICS WHERE USERNAME = '" + playerName + "';", countCallBack, &totalGames);
    return (int)totalGames;
}

int SqliteDatabase::getPlayerScore(const PlayerResult& result)
{
    return ((float)result.correctAnswerCount / (float)(result.correctAnswerCount + result.wrongAnswerCount) / result.averageAnswerTime * 1000);
}

int SqliteDatabase::getTotalPlayerScore(const std::string& username)
{
    if (getNumOfTotalAnswers(username) == 0 || getPlayerAverageAnswerTime(username) == 0 || getNumOfPlayerGames(username) == 0)
    {
        return 0;
    }
    return ((float)getNumOfCorrectAnswers(username) / (float)getNumOfTotalAnswers(username) / (float)getPlayerAverageAnswerTime(username) * getNumOfPlayerGames(username) * 250);
}

std::vector<UserScore> SqliteDatabase::getHighScores()
{
    std::vector<UserScore> scores;
    std::vector<std::string> leadingUsers;
    std::vector<std::string> users;
    executeOperation("SELECT USERNAME FROM Statistics;", usernameCallBack, &users);
    for (auto& user : users)
    {
        scores.emplace_back(user, getTotalPlayerScore(user)); // get user score and place in a struct
    }
    std::sort(scores.begin(), scores.end(), [](const UserScore& a, const UserScore& b) //sorting the user,scores vector by the score in the descending order
    {
        return a.score > b.score;
    });
    return scores;
}

int SqliteDatabase::submitGameStatistics(GameData data)
{
    
    float avgTime = 0;
    try
    {
        executeOperation("UPDATE Statistics SET GAMES_AMOUNT = GAMES_AMOUNT + 1 WHERE USERNAME = '" + data.username + "';"); // update games amount
        executeOperation("SELECT AVG_TIME FROM Statistics WHERE USERNAME = '" + data.username + "';", countCallBack, &avgTime);
        executeOperation("UPDATE Statistics SET CORRECT_ANS = CORRECT_ANS + " + std::to_string(data.correctAnswerCount) + " WHERE USERNAME = '" + data.username + "';"); // update correct
        executeOperation("UPDATE Statistics SET TOTAL_ANS = TOTAL_ANS + " + std::to_string(data.correctAnswerCount + data.wrongAnswerCount) + " WHERE USERNAME = '" + data.username + "';"); // update total
        executeOperation("UPDATE Statistics SET AVG_TIME = " + std::to_string((float)(avgTime + data.averageAnswerTime) / 2) + " WHERE USERNAME = '" + data.username + "';"); // update avg time
    }
    catch (std::exception e)
    {
        return 1;
    }
    return 0;
}

int SqliteDatabase::questionCallBack(void* data, int argc, char** argv, char** azColName)
{
    std::list<Question>* questions = reinterpret_cast<std::list<Question>*>(data);
    Question newQuestion(argv[0], argv[1], argv[2], argv[3], argv[4]);
    questions->push_back(newQuestion);
    return 0;
}

int SqliteDatabase::usernameCallBack(void* data, int argc, char** argv, char** azColName)
{
    std::vector<std::string>* users = reinterpret_cast<std::vector<std::string>*>(data);
    std::string username = argv[0];
    users->push_back(username);
    return 0;
}


std::string SqliteDatabase::decodeHtmlEntities(const std::string& str) // this function handle special chars like " or '
{
    std::unordered_map<std::string, std::string> htmlEntities = 
    {
        {"&quot;", "\""},
        {"&apos;", "'"},
        {"&amp;", "&"},
        {"&lt;", "<"},
        {"&gt;", ">"},
        {"&#039;", "'"},
        {"&#39;", "'"}
    };

    std::string result = str;
    for (const auto& entity : htmlEntities)  // replace all special chars
    {
        size_t pos = result.find(entity.first);
        while (pos != std::string::npos) 
        {
            result.replace(pos, entity.first.length(), entity.second);
            pos = result.find(entity.first, pos + entity.second.length());
        }
    }

    std::regex numericEntityPattern("&#(\\d+);");
    std::smatch match;
    std::string processedResult;
    std::string::const_iterator searchStart(result.cbegin());
    while (std::regex_search(searchStart, result.cend(), match, numericEntityPattern)) 
    {
        processedResult.append(match.prefix().first, match.prefix().second);
        char ch = static_cast<char>(std::stoi(match[1].str()));
        processedResult.push_back(ch);
        searchStart = match.suffix().first;
    }
    processedResult.append(searchStart, result.cend());

    return processedResult;
}
void SqliteDatabase::initiallzeDB()
{
    executeOperation("CREATE TABLE Users (USERNAME STRING NOT NULL PRIMARY KEY, PASSWORD STRING NOT NULL, EMAIL STRING NOT NULL);");
    executeOperation("CREATE TABLE Questions (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, QUESTION TEXT NOT NULL, CORRECT_ANSWER TEXT NOT NULL, INCORRECT_ANSWER_1 TEXT NOT NULL, INCORRECT_ANSWER_2 TEXT NOT NULL, INCORRECT_ANSWER_3 TEXT NOT NULL);");
    executeOperation("CREATE TABLE Statistics (USERNAME STRING NOT NULL, AVG_TIME FLOAT NOT NULL, TOTAL_ANS INTEGER NOT NULL, CORRECT_ANS INTEGER NOT NULL, GAMES_AMOUNT INTEGER NOT NULL, FOREIGN KEY (USERNAME) REFERENCES Users(USERNAME));");
    HINTERNET hInternet = InternetOpenA("Trivia", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) 
    {
        std::cerr << "Failed to initialize WinINet." << std::endl;
        return;
    }

    HINTERNET hConnect = InternetOpenUrlA(hInternet, TRIVIA_SITE, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) 
    {
        std::cerr << "Failed to open URL." << std::endl;
        InternetCloseHandle(hInternet);
        return;
    }

    std::string jsonContent;
    char buffer[10240];
    DWORD bytesRead;
    while 
    (
        InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        jsonContent.append(buffer, bytesRead);
    }
    const char* insertQuery = "INSERT INTO Questions (QUESTION, CORRECT_ANSWER, INCORRECT_ANSWER_1, INCORRECT_ANSWER_2, INCORRECT_ANSWER_3) VALUES (?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(m_db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) 
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(m_db) << std::endl;
        return;
    }
    json jsonObject = json::parse(jsonContent);
    for (const auto& result : jsonObject["results"]) 
    {
        std::string question = decodeHtmlEntities(result["question"].get<std::string>());
        std::string correctAnswer = decodeHtmlEntities(result["correct_answer"].get<std::string>());
        std::string incorrectAnswer1 = decodeHtmlEntities(result["incorrect_answers"][0].get<std::string>());
        std::string incorrectAnswer2 = decodeHtmlEntities(result["incorrect_answers"][1].get<std::string>());
        std::string incorrectAnswer3 = decodeHtmlEntities(result["incorrect_answers"][2].get<std::string>());

        sqlite3_bind_text(stmt, 1, question.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, correctAnswer.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 3, incorrectAnswer1.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, incorrectAnswer2.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, incorrectAnswer3.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(m_db) << std::endl;
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int SqliteDatabase::countCallBack(void* data, int argc, char** argv, char** azColName)
{
    float* result = reinterpret_cast<float*>(data);
    *result = atof(argv[0]);
    return 0;
}

void SqliteDatabase::CreateDB()
{
    int res = sqlite3_open(DBName, &m_db);
    if (m_db == nullptr)
    {
        return;
    }
    initiallzeDB();
}

bool SqliteDatabase::open()
{
    
    int file_exist = _access(DBName, 0);
    if (file_exist != 0)
        return false;

    int open_result = sqlite3_open(DBName, &m_db);
    if (open_result != SQLITE_OK)
        return false;

    if (m_db == nullptr)
        return false;

    return true;
}

void SqliteDatabase::executeOperation(const std::string& command, int(*callback)(void*, int, char**, char**), void* data)
{
    const char* cmd = command.c_str();
    char* errMsg = nullptr;
    int res = sqlite3_exec(m_db, cmd, callback, data, &errMsg);
    if (res != SQLITE_OK)
        std::cerr << "query failed: " << errMsg << "\n";

}

