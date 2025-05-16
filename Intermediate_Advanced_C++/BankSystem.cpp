#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <conio.h>
using namespace std;

unordered_map<string, string> LoadUsers() {
	unordered_map<string, string> users;
	ifstream file("users.txt");
	string username, password;

	while (file >> username >> password) {
		users[username] = password;
	}
	return users;
}

unordered_map<string, double> LoadBalance() {
	unordered_map<string, double> balances;
	string username;
	double balance;


	ifstream file("balances.txt");
	
	while (file >> username >> balance) {
		balances[username] = balance;
	}
	return balances;
}


void saveBalances(const unordered_map<string, double>& balances) {
	ofstream file("balances.txt");
	for (auto& pair : balances) {
		file << pair.first << " " << pair.second << "\n";
	}
}


void Registeration() {
	string username, password;
	char ch;

	cout << "Enter New username: ";
	cin >> username;
	cout << "Enter New Password: ";
	
	while ((ch = _getch()) != 13) {
		if (ch == 8 && !password.empty()) {
			password.pop_back();
			cout << "\b \b";
		}
		else {
			password += ch;
			cout << "*";
		}
	}

	cout << endl;

	ofstream file("users.txt", ios::app); // append mode
	file << username << " " << password << endl;

	cout << "Registaretion Success." << endl;

}

string LoginUser() {
	string username, password;
	char ch;
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	while ((ch = _getch()) != 13) {
		if (ch == 8 && !password.empty()) {
			password.pop_back();
			cout << "\b \b";
		}
		else {
			password += ch;
			cout << "*";
		}
	}

	cout << endl;

	unordered_map<string, string> users = LoadUsers();
	
	if (users.count(username) && users[username] == password) {
		cout << "Login successful! Welcome," << username << "!\n";
		return username;
	}else {
		cout << "Invalid username or password!\n";
		return "";
	}

}

void logTransaction(const string& username, const string& message) {
	string filename = "transactions/" + username + "_history.txt";
	ofstream file(filename);
	if (file.is_open()) {
		file << message << endl;
	};
}

void BankMenu(const string& username) {
	unordered_map<string, double> balances = LoadBalance();
	if (!balances.count(username)) balances[username] = 0.0;
	

	int choise;
	double amount;


	while (true) {
		cout << "\n======== Bank Menu =========\n";
		cout << "1. View Balance\n2. Deposit\n3. Withdraw\n4. View Transaction History \n5. Logging out \nchoose: ";
		cin >> choise;


		switch (choise) {
		case 1: 
			cout << "Yor Balance: $" << balances[username] << endl;
			break;
		case 2: 
			cout << "Enter amount to deposit: $";
			cin >> amount;
			balances[username] += amount;
			cout << "Deposit Success.\n";
			logTransaction(username, "Deposit: $" + to_string(amount));
			break;
		case 3: 
			cout << "Enter amout to withdraw $";
			cin >> amount;
			if (amount > balances[username]) {
				cout << "Insufficent funds!\n";
			}
			else {
				balances[username] -= amount;
				cout << "Withdraw successful.\n";
				logTransaction(username, "Withdraw: $" + to_string(amount));
			}
			break;
		case 4: {
			string filename = "transactions/" + username + "history.txt";
			ifstream historyFile(filename);
			string line;

			cout << "\n----- History Transactions -------\n";
			if (historyFile.is_open()) {
				while (getline(historyFile, line)) {
					cout << line << endl;
				}
				historyFile.close();
			}
			else {
				cout << "no history found. \n";
			}
			break;
		}
		case 5: 
			saveBalances(balances);
			cout << "Logging out ....\n";
			return;
		default: 
			cout << "Invalid option.";
		}
		
	}
}

void adminMenu() {
	int choice;
	while (true) {
		cout << "\n=== Admin Menu ===\n";
		cout << "1. View All Users\n";
		cout << "2. View All Balances\n";
		cout << "3. View User Transaction History\n";
		cout << "4. Logout\nChoose: ";
		cin >> choice;

		if (choice == 1) {
			unordered_map<string, string> users = LoadUsers();
			cout << "\nRegistered Users:\n";
			for (auto& u : users) {
				cout << "- " << u.first << endl;
			}
		}
		else if (choice == 2) {
			unordered_map<string, double> balances = LoadBalance();
			cout << "\nUser Balances:\n";
			for (auto& b : balances) {
				cout << b.first << ": $" << b.second << endl;
			}
		}
		else if (choice == 3) {
			string targetUser;
			cout << "Enter username to view history: ";
			cin >> targetUser;
			string filename = "transactions/" + targetUser + "_history.txt";
			ifstream file(filename);
			string line;
			cout << "\nTransaction History for " << targetUser << ":\n";
			if (file.is_open()) {
				while (getline(file, line)) {
					cout << line << endl;
				}
				file.close();
			}
			else {
				cout << "No history found for that user.\n";
			}
		}
		else if (choice == 4) {
			cout << "Logging out admin...\n";
			return;
		}
		else {
			cout << "Invalid choice.\n";
		}
	}
}


int main() {
	int choice;
	bool is_Running = true;

	while (is_Running) {
		cout << "\n ========= Bank Menu ==========\n";
		cout << "1. Regster \n2. Login  \n3. Exit \nChoose: ",
			cin >> choice;

		switch (choice) {
		case 1: Registeration(); break;
		case 2: {
			string is_Admin = "admin";
			string user = LoginUser();
			if (!user.empty()) {
				if (user == is_Admin) {
					adminMenu();
				}
				else {
				BankMenu(user);
				}
			}
			break;
			}
		case 3:
			is_Running = false;
			cout << "\n Goodbye!\n";
			break;
		default: cout << "Invalid Option. \n";
		}
	}

	return 0;
}