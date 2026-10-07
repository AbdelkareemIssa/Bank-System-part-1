// L7#Project2(BankSystem).cpp : This file contains the 'main' function. Program execution begins and ends there.
//const string ClientFileName = "Clients.txt";



#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

const string ClientFileName = "Clients.txt";

enum enMainMenueOptions {
	ShowClients = 1, addClient = 2,
	deleteClient = 3, UpdateClient = 4,
	FindClient = 5,Transaction=6,
	Exit = 7
};

enum enTransactionOptions {
	Deposit = 1, WithDraw = 2,
	TotalBslance = 3, MainMenue = 4 
};

int ReadNumber(string Mess, int from, int to)
{
	int num;
	do
	{
		cout << Mess;
		cin >> num;
	} while (num<from || num>to);


	return num;
}

enMainMenueOptions ChoiceList()
{
	int MainMenue = ReadNumber("Choose what do you want to do[1 To 7]", 1, 7);

	switch (MainMenue)
	{
	case 1:
		return enMainMenueOptions::ShowClients;
		break;
	case 2:
		return enMainMenueOptions::addClient;
		break;
	case 3:
		return enMainMenueOptions::deleteClient;
		break;
	case 4:
		return enMainMenueOptions::UpdateClient;
		break;
	case 5:
		return enMainMenueOptions::FindClient;
		break;
	case 6:
		return enMainMenueOptions::Transaction;
		break;
	case 7:
		return enMainMenueOptions::Exit;
		break;


	}

}

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkDelete = false;
};

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable
	// use find() function to get the position of the delimiters
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, pos + Delim.length()); /* erase() until
		positon and move to next word. */
	}
	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}
	return vString;
}

sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string>VS = SplitString(Line, Seperator);
	Client.AccountNumber = VS[0];
	Client.PinCode = VS[1];
	Client.Name = VS[2];
	Client.Phone = VS[3];
	Client.AccountBalance = stod(VS[4]);

	return Client;
}

vector<sClient>LoadClientDataFromFile(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	string Line;
	vector<sClient>VClients;

	while (getline(MyFile, Line))
	{
		VClients.push_back(ConvertLineToRecord(Line));
	}

	return VClients;
}

void PrintClientCard(sClient client)
{
	cout << client.AccountNumber << "		" << client.PinCode << "		" << client.Name << "		" << client.Phone << "		" << client.AccountBalance << endl;
}

void PrintClientCardForBalance(sClient client)
{
	cout << client.AccountNumber << "		" << client.Name <<"		" << client.AccountBalance << endl;
}

void PrintAllClients()
{
	vector <sClient> vClients = LoadClientDataFromFile(ClientFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (sClient Client : vClients)
		{
			PrintClientCard(Client);
			cout << endl;
		}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

bool ClientExistsAccountNumber(string AccountNumber, string FileName)
{
	vector<sClient> clients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient client;
		while (getline(MyFile, Line))
		{
			client = ConvertLineToRecord(Line);
			if (client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			clients.push_back(client);
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient Client;
	cout << "Enter AccountNumber?";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsAccountNumber(Client.AccountNumber, ClientFileName))
	{
		cout << "\nClient with[" << Client.AccountNumber << "]already exists,Enter another account number";
		getline(cin >> ws, Client.AccountNumber);

	}
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}

string ConvertRecordToLine(sClient Client, string seprator = "#//#")
{
	string Line;

	Line += Client.AccountNumber + seprator;
	Line += Client.PinCode + seprator;
	Line += Client.Name + seprator;
	Line += Client.Phone + seprator;
	Line += to_string(Client.AccountBalance);
	return Line;
}

void AddDataLineToFile(string FileName, string Line)
{
	fstream MyFile;
	MyFile.open(FileName, ios::app);

	if (MyFile.is_open())
	{
		MyFile << Line << endl;
		MyFile.close();
	}

}

bool FindClientByAccountNumber(string AccountNumber, sClient& client, vector<sClient>VRecord)
{


	for (sClient C : VRecord)
	{
		if (AccountNumber == C.AccountNumber)
		{
			client = C;
			return true;
		}
	}
	return false;
}

void AddNewClient()
{
	sClient client;
	client = ReadNewClient();
	AddDataLineToFile(ClientFileName, ConvertRecordToLine(client));


}

void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding new client:\n\n";

		AddNewClient();
		cout << "\nClient Added successfully,do you want to add more clients Y/N/";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewClientScreen()
{
	cout << "	------------------------------------------\n";
	cout << "		   Add new clients screen<<\n";
	cout << "	------------------------------------------\n";
	AddNewClients();
}

void printShowMenue()
{
	system("cls");
	cout << "======================================" << endl;
	cout << "				Main Menue" << endl;
	cout << "======================================" << endl;
	cout << "		[1] Show Clients List." << endl;
	cout << "		[2] Add new Client" << endl;
	cout << "		[3] Delete Client" << endl;
	cout << "		[4] Update Client" << endl;
	cout << "		[5] Find Client" << endl;
	cout << "		[6] Transaction" << endl;
	cout << "		[7] Exit Client" << endl;
	cout << "======================================" << endl;
}

bool MarkOnClientDataToDelete(string AccountNumber, vector<sClient>& VClients)
{
	for (sClient& C : VClients)
	{
		if (AccountNumber == C.AccountNumber)
		{
			C.MarkDelete = true;
			return true;
		}
	}
	return false;
}

void SaveClientDataToFile(string FileName, vector<sClient>& VClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);
	for (sClient& C : VClients)
	{
		if (C.MarkDelete == false)
		{
			MyFile << (ConvertRecordToLine(C)) << endl;
		}

	}
	MyFile.close();

}

sClient ChangeClientData(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name); 
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}

bool DeleteClientDataByAccountNumber(string  AccountNumber, vector<sClient>& VClients)
{
	sClient client;

	char answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, client, VClients))
	{
		PrintClientCard(client);

		cout << "you want delete if ya enter Y/y";

		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			MarkOnClientDataToDelete(AccountNumber, VClients);
			SaveClientDataToFile(ClientFileName, VClients);
		}
		return true;
	}
	else
	{
		cout << "The Client is not found";
		return false;
	}
}

bool UpdateClientDataByAccountNumber(string  AccountNumber, vector<sClient>& VClients)
{
	sClient client;
	char answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, client, VClients))
	{
		PrintClientCard(client);

		cout << "Want to Update The client if ya enter Y/y";

		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			for (sClient& C : VClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientData(AccountNumber);
					break;
				}

			}
			SaveClientDataToFile(ClientFileName, VClients);
			return true;
		}

	}
	else
	{
		cout << "The client is not found";
		return false;
	}

}

void ShowMainMenue();

void GoBackToMenue()
{
	cout << "\n\nPress any key to go back to main menue ....";
	system("pause>0");
	ShowMainMenue();

}

void ShowTransactionScreen();

void GoBackToTransaction()
{
	cout << "\n\nPress any key to go back to Transaction menue ....";
	system("pause>0");
	ShowTransactionScreen();

}

short ReadMainMenueOption()
{
	cout << "Choose what do you to do?[1 to 7]?";
	short choice = 0;
	cin >> choice;

	return choice;
}

string ReadString(string Mess)
{
	cout << Mess;
	string s1;
	cin >> s1;
	return s1;

}

void ShowDeleteClientScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tDelete client screen\n";
	cout << "\n----------------------------\n";

	vector<sClient>vclient = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadString("Please enter account number");
	DeleteClientDataByAccountNumber(AccountNumber, vclient);
}

void ShowUpdateClientScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tUpdate client screen\n";
	cout << "\n----------------------------\n";

	vector<sClient>vclient = LoadClientDataFromFile(ClientFileName);
	string AccountNumber = ReadString("Please enter account number");
	UpdateClientDataByAccountNumber(AccountNumber, vclient);

}

void ShowFindClientScreen()
{
	cout << "\n----------------------------\n";
	cout << "\t Find client screen\n";
	cout << "\n----------------------------\n";

	vector<sClient>vclient = LoadClientDataFromFile(ClientFileName);
	sClient client;
	string AccountNumberClient = ReadString("Please account number");
	if (FindClientByAccountNumber(AccountNumberClient, client, vclient))
	{
		PrintClientCard(client);
	}
	else
	{
		cout << "\nClient with accountnumber[" << AccountNumberClient << "] is not found";
	}
}

short ReadTransactionsMenueOption()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount ,vector<sClient>Vclient)
{
	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this transaction? y / n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (sClient &C : Vclient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance+=Amount;
				SaveClientDataToFile(ClientFileName,Vclient);
				cout << "\n\nDone Successfully. New balance is: "<< C.AccountBalance;
				return true;
			}
		}
		return false;
	}

}

void ShowDeposit()
{
	vector<sClient>VClient = LoadClientDataFromFile(ClientFileName);

	string AccountNumber= ReadString("Plase account number: \n");

	sClient Client;

	

	while (!FindClientByAccountNumber(AccountNumber,Client,VClient))
	{
		cout<<"Client with["<<AccountNumber<<"]does not exist.";
		AccountNumber = ReadString("Plase account number: \n");
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber,Amount,VClient);


}

bool WithDrawBalanceToClientByAccountNumber(string AcountNumber,vector<sClient>Vclient,double Amount)
{

	char Answer = 'n';

	cout << "\n\nAre you sure you want perfrom this transaction? y / n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (sClient &C:Vclient)
		{
			if (AcountNumber == C.AccountNumber)
			{
				while (C.AccountBalance < Amount)
				{
					cout<<"Amount Exceeds the balance, you can withdraw up to :"<<C.AccountBalance;
					cout<<"Please enter another amount ? ";
					cin>>Amount;
				}
				C.AccountBalance-=Amount;
				SaveClientDataToFile(ClientFileName,Vclient);
				return true;
			}
		}
	}
	return false;
	
}

void ShowWithDraw()
{
	vector<sClient>VClient = LoadClientDataFromFile(ClientFileName);

	string AccountNumber = ReadString("Plase account number: \n");

	sClient Client;

	while (!FindClientByAccountNumber(AccountNumber, Client, VClient))
	{
		cout << "Client with[" << AccountNumber << "]does not exist.";
		AccountNumber = ReadString("Plase account number: \n");
	}
	PrintClientCard(Client);


	double Amount = 0;
	cout << "\nPlease enter WithDraw amount? ";
	cin >> Amount;
	WithDrawBalanceToClientByAccountNumber(AccountNumber,VClient,Amount);


}

void ShowTotalBalance()
{

	vector <sClient> vClients = LoadClientDataFromFile(ClientFileName);
	
	double TotalBalances=0;

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name"; 
	cout << "| " << left << setw(12) << "Balance";
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (sClient Client : vClients)
		{
			PrintClientCardForBalance(Client);
			TotalBalances+=Client.AccountBalance;
			cout << endl;
		}
	cout <<
		"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout<<"Total balnces"<<TotalBalances;


}

void PerfromTransactionOption(enTransactionOptions  TransactionOptions)
{
	switch (TransactionOptions)
	{
		case enTransactionOptions::Deposit:
		system("cls");
		ShowDeposit();
		GoBackToTransaction();
		break;
		case enTransactionOptions::WithDraw:
		ShowWithDraw();
		GoBackToTransaction();
		break;
		case enTransactionOptions::TotalBslance:
		system("cls");
		ShowTotalBalance();
		GoBackToTransaction();
		break;
		case enTransactionOptions::MainMenue:
		system("cls");
		ShowMainMenue();
		break;
		 
	}

}

void ShowTransactionScreen()
{ 
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n ";

	PerfromTransactionOption((enTransactionOptions) ReadTransactionsMenueOption());



}

void ShowExitScreen()
{
	cout << "\n----------------------------\n";
	cout << "\t Program End\n";
	cout << "\n----------------------------\n";
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOptions)

{
	switch (MainMenueOptions)
	{
	case enMainMenueOptions::ShowClients:
		system("cls");
		PrintAllClients();
		GoBackToMenue();
		break;
	case enMainMenueOptions::addClient:
		system("cls");
		AddNewClients();
		GoBackToMenue();
		break;
	case enMainMenueOptions::deleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMenue();
		break;
	case enMainMenueOptions::UpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMenue();
		break;
	case enMainMenueOptions::FindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMenue();
		break;
	case enMainMenueOptions::Transaction:
		system("cls");
		ShowTransactionScreen();
		break;
	case enMainMenueOptions::Exit:
		system("cls");
		ShowExitScreen();
		break;
	}

}

void ShowMainMenue()
{
	printShowMenue();
	PerfromMainMenueOption(enMainMenueOptions(ReadMainMenueOption()));
}

int main()

{

	ShowMainMenue();
}
