#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include<windows.h>  
using namespace std;

template<typename C, typename T>
bool contains(C&& c, T e) {
	return find(begin(c), end(c), e) != end(c);
}
//AUTOR: PATRYK KLIMKIEWICZ
class HANGMAN {
public:
	unsigned int etap;
	string steps[11] = {};
	char wrongguesses[11] = {};
	char correctguesses[100] = {};
	string words[851] = {};
	string currentword = "";
	HANGMAN() 
	{
		this->etap = 0;
		{
			this->steps[0] = "\n\n\n\n\n\n\n\n-----------";
			this->steps[1] = "\n   -\n   |\n   |\n   |\n   |\n   |\n   |\n-----------";
			this->steps[2] = "\n   -----\n   |\n   |\n   |\n   |\n   |\n   |\n-----------";
			this->steps[3] = "\n   -----\n   |   |\n   |\n   |\n   |\n   |\n   |\n-----------";
			this->steps[4] = "\n   -----\n   |   |\n   |   O\n   |\n   |\n   |\n   |\n-----------";
			this->steps[5] = "\n   -----\n   |   |\n   |   O\n   |   |\n   |\n   |\n   |\n-----------";
			this->steps[6] = "\n   -----\n   |   |\n   |   O\n   |  /|\n   |\n   |\n   |\n-----------";
			this->steps[7] = "\n   -----\n   |   |\n   |   O\n   |  /|\\\n   |\n   |\n   |\n-----------";
			this->steps[8] = "\n   -----\n   |   |\n   |   O\n   |  /|\\\n   |   ^\n   |\n   |\n-----------";
			this->steps[9] = "\n   -----\n   |   |\n   |   O\n   |  /|\\\n   |   ^\n   |  / \n   |\n-----------";
			this->steps[10] = "\n   -----\n   |   |\n   |   O\n   |  /|\\\n   |   ^\n   |  / \\\n   |\n-----------";
		}
		fstream file;
		{
			file.open("words.txt", ios::in);
			if (file.is_open()) {
				string tp;
				int i = 0;
				while (getline(file, tp)) {
					this->words[i++] = tp;
				}
				file.close();
			}
		}
		this->shakeup();
	}
	void shakeup() {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distr(0, 851);
		int rando = distr(gen);
		this->currentword = this->words[rando];
	}
	void display(bool stillplaing) {
		system("CLS");
		if (stillplaing) {
			cout << "You have ten possible mistakes" << endl;
			cout << "Current word: ";
			for (int i = 0; i < this->currentword.size(); i++) {
				if (contains(this->correctguesses, this->currentword[i])) {
					cout << this->currentword[i] << " ";
				}
				else {
					cout << "_ ";
				}
			}
			cout << this->steps[this->etap];
		}
		else {
			cout << "You just lost!" << endl;
			cout << "The word was: ";
			for (int i = 0; i < this->currentword.size(); i++) {
				cout << this->currentword[i] << " ";
			}
			cout << this->steps[this->etap];
		}
	}
	bool play() {
		if (strlen(this->correctguesses) == this->currentword.size()) {
			return true;
		}
		cout << endl << "Would you like to guess the word?\nPlease enter a string starting with 1 if you do, anything else is a no: ";
		char yes = '0';
		cin.clear();
		cin >> yes;
		cin.ignore(10000, '\n');
		if (yes == '1') {
			string guess;
			cout << "Please enter your guess:";
			cin.clear();
			cin >> guess;
			 cin.ignore(10000, '\n');
			if (guess == this->currentword) {
				return true;
			}
			else {
				this->etap++;
				return false;
			}
		}
		else {
			char guess;
			while (true) {
				cout << endl << "Guess a letter: ";
				try {
					cin.clear();
					cin >> guess;
					 cin.ignore(10000, '\n');
				}
				catch (...) {
					continue;
				}
				if (contains(this->correctguesses, guess) || contains(this->wrongguesses, guess)) {
					cout << "you have already guessed that letter";
					continue;
				}
				else {
					break;
				}
			}
			if (contains(this->currentword, guess)) {
				int nextindex = strlen(this->correctguesses);
				this->correctguesses[nextindex] = guess;
			}
			else {
				int nextindex = strlen(this->wrongguesses);
				this->wrongguesses[nextindex] = guess;
				if (this->etap >= 10) {
					return true;
				}
				else {
					this->etap++;
				}
			}
			return false;
		}
	}
	void run() {
		while (strlen(this->wrongguesses) <= 10 or this->etap<=10) {
			this->display(true);
			if (this->play()) {
				break;
			}
		}
		if (strlen(this->wrongguesses) >= 10 or this->etap >= 10) {
			this->display(false);
		}
		else {
			system("CLS");
			cout << "Congratulations! You've won!"<<endl;
			cout << "Current word: ";
			for (int i = 0; i < this->currentword.size(); i++) {
				cout << this->currentword[i] << " ";
			}
			cout << this->steps[this->etap];
		}
		if (this->restart()) {
			this->shakeup();
			this->etap = 0;
			fill(begin(this->correctguesses), end(this->correctguesses), 0);
			fill(begin(this->wrongguesses), end(this->wrongguesses), 0);
			this->run();
		}
		return;
	}
	bool restart() {
		cout << "\nWould you like to play another round? [1 / anything]: ";
		bool temp;
		cin.clear();
		cin >> temp;
		cin.ignore(10000, '\n');
		return temp;
	}
};

int main()
{
	HANGMAN h;
	h.run();
	return 0;
}
