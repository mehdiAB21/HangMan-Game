#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <windows.h>
using namespace std;

// DB //
vector<vector<string>> mainDB = {{"animal", "cat"}, {"fruit", "apple"}, {"country", "canada"}, {"color", "blue"},  {"vehicle", "bicycle"}, {"city", "paris"}, {"sport", "soccer"}, {"flower", "tulip"}, {"instrument", "guitar"}, {"profession", "doctor"}}; 
vector<vector<string>> words = mainDB; 
int score = 0;
// end DB




string color(const string& text, int c)
{

    // blue: 34, red: 31, green: 32, yellow: 33, 


    return "\033[" + to_string(c) + "m" + text + "\033[0m";
}


void Remove_hints(string& word_n, char guess_char)
{
    for (int i = 0; i < word_n.size(); ) {
        if (word_n[i] == guess_char){
            word_n.erase(i, 1);   // delete char inex i
            break;
        }
        else
            i++;
    }
}


bool check_letter_on_word(vector<string> &guessLetter, string word, char guess_char){
    for(int i = 0; i < word.length(); i++){
        if(word[i] == guess_char && guessLetter[i] == "_"){
            guessLetter[i] = guess_char;
            return true;
        }
    }

    return false;
}


// Board | start //

string first_and_last_border( int cols)
{
    string border;
    for (int i = 0; i < cols; ++i)
    {
        border += "-";
    }
    border += "\n";
    return border;
}
vector<string> locationDetection(int col)
{
    vector<string> cols(col);

    for (int i = 0; i < col; i++) {
        if (i == 0 || i == col - 1) cols[i] = '|';
        else cols[i] = ' ';
    }

    return cols;
}

vector<vector<string>> createBoard(int rows, int cols)
{
    vector<vector<string>> board(rows, locationDetection(cols));
    return board;
}


void printBoard(const vector<vector<string>>& board)
{
    for (auto& row : board) {
    	for(string c : row) cout << c;
            cout << endl; 
    }
}

void put_message(vector<vector<string>>& board, string text, int colorCode)
{
    int cols = board[0].size();
    int startCol = (cols - text.length()) / 2;

    for(int i = 0; i < text.length(); i++){
        board[12][startCol + i] = color(string(1, text[i]), colorCode);
    }
}

void put_hintsOnBoard(vector<vector<string>>& board, int hints){

    string text = "Hints:";
    for(int i = 0; i < 7; i++){
        board[0][i + 1] = text[i];
    }

    string hint = to_string(hints);

    board[0][7] = hint;

}


// Board | end //

// random | start //

int random(int size)
{
    int R = rand() % size;
    return R;
}


// random | end // 


// Text on board  start //

void put_textOnBoard(vector<vector<string>>& board, string text){

    int cols = board[0].size();
    int startCol = (cols - text.length()) / 2;
    

    for(int i = 0; i < text.length(); i++){
        board[3][startCol + i] = text[i];
    }

}



// Text on board || end //

// Hidden word || start //

void put_hiddenword(vector<vector<string>> &board, vector<string> line)
{
    int cols = board[0].size();
    int startCol = (cols - (2 * line.size() - 1)) / 2;
    for (int i = 0; i < 2 * line.size() - 1; i++)
    {
        if (i % 2 == 0)
            board[8][i + startCol] = line[i / 2];
        if (i % 2 == 1)
            board[8][i + startCol] = ' ';
    }
}



// check Win 


bool checkWin(const vector<string>& guess_letter)
{
    for(string c : guess_letter){
        if(c == "_"){
            return false;
        }
    }
    return true;
}

void delete_word_of_list(vector<vector<string>> &words, int randomNum)
{
    words.erase(words.begin() + randomNum);
}

int main()
{
    srand(time(0)); // for random func

    
    
    string request;
    

    int randomNum = random(words.size());
    auto board = createBoard(15, 40);
    string word = words[randomNum][1];
    int word_size = word.size();
    int allowedNum = word_size;
    int hintNum = ceil(word_size / 3.0); 

    string word_n = word;

    vector<string> guess_letter(word_size,"_");

    int runCount = 0;

    while (!words.empty())
    {

        if (allowedNum <= 0)
        {
            system("cls");

            cout << endl;
            cout << first_and_last_border(40);

            put_textOnBoard(board, words[randomNum][0]);
            put_hintsOnBoard(board, hintNum);
            put_hiddenword(board, guess_letter);
            put_message(board, "GAME OVER !", 31);
            printBoard(board);
            cout << first_and_last_border(40);
            cout << endl;

            Sleep(3000); 
            words = mainDB;
            system("cls");
            main();
        }

        system("cls");

        cout << endl;
        cout << first_and_last_border(40);

        put_textOnBoard(board, words[randomNum][0]);
        put_hintsOnBoard(board, hintNum);
        put_hiddenword(board, guess_letter);
        if(runCount++ == 0){
            put_message(board, "You can :)", 34);
        }
        printBoard(board);
        cout << first_and_last_border(40);
        cout << endl;

        if (checkWin(guess_letter))
        {
            system("cls");

            cout << endl;
            cout << first_and_last_border(40);

            put_textOnBoard(board, words[randomNum][0]);
            put_hintsOnBoard(board, hintNum);
            put_hiddenword(board, guess_letter);
            put_message(board, "YOU WIN round :)", 32);
            printBoard(board);
            cout << first_and_last_border(40);
            cout << endl;

            delete_word_of_list(words, randomNum);
            score += 3;
            Sleep(2000); 
            system("cls");
            if(words.empty()){
                break;
            }
            main();
        }

        cin >> request;
        if (request == "hint")
        {
            if (hintNum > 0)
            {
                char guess_char = word_n[random(word_n.length())];
                Remove_hints(word_n, guess_char);

                allowedNum--;
                hintNum--;
                score--;
                put_message(board, "                ", 33);
                put_message(board, "Hint used!", 33);
                check_letter_on_word(guess_letter, word, guess_char);
            }
            else
            {
                put_message(board, "No hints left!", 31);
            }
        }else if(request == "exit"){
            return 0;
        }
        else
        {
            char guess_char = request[0];
            if (!check_letter_on_word(guess_letter, word, guess_char))
            {
                allowedNum--;
                put_message(board, "Wrong guess!", 31);
            }
            else
            {
                Remove_hints(word_n, guess_char); // now i write
                put_message(board, "                   ", 32);
                put_message(board, "Good guess!", 32);
                
            }
        }
    }

    cout << "You Win Game :)" << endl;
    cout << "Your Score is : " << score << endl;

    return 0;
}
