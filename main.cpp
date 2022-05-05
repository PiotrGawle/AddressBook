#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

struct Contact{
    int idRecipient, idUser;
    string name, surname, phone, address, email;
};
struct User{
    int idUser;
    string nick, password;
};

int loadUsersFromFile (vector<User> &users);
int registration (vector<User> &users, int numberOfUsers);
int logging (vector<User> &users, int numberOfUsers);
void moveToAddressBook(vector <User> &users, int &numberOfUsers, int idOfLoggedUser);
void addUserToFile (vector<User> users, int numberOfUsers);
void overwriteUsersFile (vector<User> users);
void changePassword (vector<User> &users, int numberOfUsers, int idOfLoggedUser);
int readContactsFromFile (vector<Contact> &contacts, int idOfLoggedUser);
void addContact (vector<Contact> &contacts, int &numberOfUserContacts, int idOfLoggedUser);
void addContactToFile (vector<Contact> &contacts, int numberOfUserContacts);
void searchByName (vector<Contact> contacts);
void searchBySurname (vector<Contact> contacts);
void overwriteRecipientFile (vector<Contact> contacts, vector<Contact>::iterator itr, string action);
void displayContact (vector<Contact> contacts, vector<Contact>::iterator itr);
void editContact (vector<Contact> &contacts);
void deleteContact (vector<Contact> &contacts, int &numberOfUserContacts);
int checkIDofTheLastContact();


int main()
{
    vector <User> users;
    int idOfLoggedUser = 0;
    int numberOfUsers = loadUsersFromFile (users);
    char choice;

    while (1)
    {
        system ("cls");
        cout << "1. Registration" << endl;
        cout << "2. Logging" << endl;
        cout << "3. Exit" << endl;
        cout << "Your choice: ";
        cin >> choice;

        if (choice == '1')
        {
            numberOfUsers = registration(users, numberOfUsers);
        }
        else if (choice == '2')
        {
            idOfLoggedUser = logging(users,numberOfUsers);
            if (idOfLoggedUser != 0)
            {
                moveToAddressBook(users,numberOfUsers,idOfLoggedUser);
            }
        }
        else if (choice == '3')
            exit(0);
    }
    return 0;
}

int loadUsersFromFile (vector<User> &users)
{
    fstream file;
    file.open("users.txt", ios::in);
    if (!file.good())
    {
        return 0;
    }
    string data;
    int numberOfUsers = 0;
    while (!file.eof())
    {
        getline(file,data,'|');
        if (data != "")
        {
            users.push_back(User());
            users[numberOfUsers].idUser = atoi(data.c_str());
            getline(file,data,'|');
            users[numberOfUsers].nick = data;
            getline(file,data);
            users[numberOfUsers].password = data;
            numberOfUsers++;
        }
    }
    file.close();
    return numberOfUsers;
}

void addUserToFile (vector<User> users, int numberOfUsers)
{
    fstream file;
    file.open("users.txt", ios::out | ios::app);

    if (numberOfUsers == 0)
    {
        file << users[numberOfUsers].idUser << "|";
    }
    else
    {
        file << endl << users[numberOfUsers].idUser << "|";
    }
    file << users[numberOfUsers].nick << "|";
    file << users[numberOfUsers].password;
    file.close();
}

int registration (vector<User> &users, int numberOfUsers)
{
    string nick, password;
    cout << "Enter nick of the User: ";
    cin >> nick;
    vector<User>::iterator itr = users.begin();
    while (itr != users.end())
    {
        if (itr -> nick == nick)
        {
            cout << "User already exists. Please enter a different nick of the User: ";
            cin >> nick;
            itr = users.begin();
        }
        else
            itr++;
    }
    cout << "Enter password: ";
    cin >> password;
    users.push_back(User());
    users[numberOfUsers].nick = nick;
    users[numberOfUsers].password = password;
    users[numberOfUsers].idUser = numberOfUsers+1;
    cout << "User has been created" << endl;
    Sleep(1000);
    addUserToFile (users, numberOfUsers);
    return ++numberOfUsers;
}

int logging (vector<User> &users, int numberOfUsers)
{
    string nick, password;
    cout << "Enter User nick: ";
    cin >> nick;

    vector<User>::iterator itr = users.begin();
    while (itr != users.end())
    {
        if (itr -> nick == nick)
        {
            for (int j=0; j<3; j++)
            {
                cout << "Enter password (" << 3-j << " chances left): ";
                cin >> password;
                if (itr -> password==password)
                {
                    cout << "You have successfully logged in." << endl;
                    Sleep (1000);
                    return itr -> idUser;
                }
            }
            cout << "Incorrect password. Wait 3 seconds until next chance." << endl;
            Sleep (3000);
            return 0;
        }
        itr++;
    }
    cout << "There is no user with that nick!" << endl;
    Sleep (1500);
    return 0;
}

void overwriteUsersFile (vector<User> users)
{
    fstream file;
    file.open("users.txt", ios::out);
    for (int i = 0; i < users.size(); i++)
    {
        if (i == 0)
        {
            file << users[i].idUser << "|";
        }
        else
        {
            file << endl << users[i].idUser << "|";
        }
        file << users[i].nick << "|";
        file << users[i].password;
    }
    file.close();
}

void changePassword (vector<User> &users, int numberOfUsers, int idOfLoggedUser)
{
    string password;
    cout << "Enter new password: ";
    cin >> password;
    for (int i=0; i<numberOfUsers; i++)
    {
        if (users[i].idUser == idOfLoggedUser)
        {
            users[i].password=password;
            overwriteUsersFile(users);
            cout << "Password has been changed." << endl;
            Sleep (1500);
        }
    }
}

string changeFirstLetter(string text)
{
    if (!text.empty())
    {
        transform(text.begin(), text.end(), text.begin(), ::tolower);
        text[0] = toupper(text[0]);
    }
    return text;
}

void overwriteRecipientFile (vector<Contact> contacts, vector<Contact>::iterator itr, string action)
{
    fstream file, tempFile;
    file.open("Recipients.txt", ios::in);
    tempFile.open("Recipients_temp.txt", ios::out);

    string line,idContact;
    while(!file.eof())
    {
        getline(file,idContact,'|');
        getline(file,line);
        if (itr -> idRecipient == atoi(idContact.c_str()) && action == "edit")
        {
            tempFile << itr -> idRecipient << "|";
            tempFile << itr -> idUser << "|";
            tempFile << itr -> name << "|";
            tempFile << itr -> surname << "|";
            tempFile << itr -> phone << "|";
            tempFile << itr -> address << "|";
            tempFile << itr -> email << endl;
        }
        else if (itr -> idRecipient == atoi(idContact.c_str()) && action == "delete")
        {
            continue;
        }
        else if (atoi(idContact.c_str()))
        {
            tempFile << idContact + "|" + line << endl;
        }
    }
    file.close();
    tempFile.close();

    remove("Recipients.txt");
    rename("Recipients_temp.txt","Recipients.txt");
}

void displayContact (vector<Contact> contacts, vector<Contact>::iterator itr)
{
    cout << itr -> idRecipient << endl;
    cout << itr -> name << endl;
    cout << itr -> surname << endl;
    cout << itr -> phone << endl;
    cout << itr -> address << endl;
    cout << itr -> email << endl << endl;
}

void editContact (vector<Contact> &contacts)
{
    int id;
    bool contactExists = false;
    char choice;
    string newData;
    cout << "Enter ID of the Contact you want to edit: ";
    cin >> id;

    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
    {
        if (itr -> idRecipient == id)
        {
            contactExists == true;
            while (1)
            {
                system ("cls");
                cout << "Contact to be edited: " << endl;
                displayContact (contacts,itr);
                cout << endl << "Chose data to edit: " << endl;
                cout << "1. Name" << endl;
                cout << "2. Surname" << endl;
                cout << "3. Phone" << endl;
                cout << "4. Email" << endl;
                cout << "5. Address" << endl;
                cout << "6. Return" << endl;
                choice = getch();
                cin.sync();

                switch (choice)
                {
                case '1':
                {
                    cout << "Enter new data: ";
                    cin >> newData;
                    itr -> name = changeFirstLetter(newData);
                    overwriteRecipientFile(contacts, itr, "edit");
                    cout << "Data has been updated." << endl;
                    Sleep (1000);
                    continue;
                }
                case '2':
                {
                    cout << "Enter new data: ";
                    getline(cin,newData);
                    itr -> surname = changeFirstLetter(newData);
                    overwriteRecipientFile(contacts, itr, "edit");
                    cout << "Data has been updated." << endl;
                    Sleep (1000);
                    continue;
                }
                case '3':
                {
                    cout << "Enter new data: ";
                    getline(cin,newData);
                    itr -> phone = changeFirstLetter(newData);
                    overwriteRecipientFile(contacts, itr, "edit");
                    cout << "Data has been updated." << endl;
                    Sleep (1000);
                    continue;
                }
                case '4':
                {
                    cout << "Enter new data: ";
                    getline(cin,newData);
                    itr -> email = changeFirstLetter(newData);
                    overwriteRecipientFile(contacts, itr, "edit");
                    cout << "Data has been updated." << endl;
                    Sleep (1000);
                    continue;
                }
                case '5':
                {
                    cout << "Enter new data: ";
                    getline(cin,newData);
                    itr -> address = newData;
                    overwriteRecipientFile(contacts, itr, "edit");
                    cout << "Data has been updated." << endl;
                    Sleep (1000);
                    continue;
                }
                case '6':
                {
                    break;
                }
                default:
                {
                    cout << "Wrong choice." << endl;
                    Sleep (1000);
                    continue;
                }
                }
                break;
            }
        }
    }
    if(contactExists == false)
    {
        cout << endl << "There is no contact with that ID!" << endl << endl;
        Sleep (2000);
    }
}

void deleteContact (vector<Contact> &contacts, int &numberOfUserContacts)
{
    int id;
    bool contactExists = false;
    cout << "Enter ID of a Contact you want to delete: ";
    cin >> id;
    for (vector<Contact>::iterator itr = contacts.begin(); itr < contacts.end(); itr++)
    {
        if (itr -> idRecipient == id)
        {
            contactExists = true;
            cout << "Contact to be removed: " << endl;
            displayContact(contacts,itr);
            cout << "Remove it? Y/N" << endl;
            if (getch() == 'Y' || getch() == 'y')
            {
                overwriteRecipientFile (contacts, itr, "deleting");
                contacts.erase(itr);
                numberOfUserContacts--;
                cout << endl << "Contact has been removed." << endl;
                Sleep(3000);
            }
            else if ((getch() == 'n' || getch() == 'N'))
                break;
        }
    }
    if (contactExists == false)
    {
        cout << endl << "There is no contact with that ID!" << endl << endl;
        Sleep (2000);
    }
}

void displayAllContacts (vector<Contact> contacts)
{

    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
    {
        displayContact(contacts,itr);
    }
    system ("pause");
}

void searchBySurname (vector<Contact> contacts)
{
    string surname;
    cout << "Enter surname of Contact you want to find: ";
    cin >> surname;

    bool contactExists = false;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
    {
        if (itr -> surname == changeFirstLetter(surname))
        {
            contactExists = true;
            displayContact(contacts,itr);
        }
    }
    if (contactExists == false)
        cout << endl << "There is no such contact!" << endl << endl;
    system ("pause");
}

void searchByName (vector<Contact> contacts)
{
    string name;
    cout << "Enter name of the contact you want to find: ";
    cin >> name;
    bool contactExists = false;
    for (vector<Contact>::iterator itr = contacts.begin(); itr != contacts.end(); itr++)
    {
        if (itr -> name == changeFirstLetter(name))
        {
            contactExists = true;
            displayContact(contacts,itr);
        }
    }
    if (contactExists == false)
        cout << endl << "There is no such Contact!" << endl << endl;
    system ("pause");
}

void addContactToFile (vector<Contact> &contacts, int numberOfUserContacts)
{
    fstream file;
    file.open("Recipients.txt", ios::out | ios::app);
    file << contacts[numberOfUserContacts].idRecipient << "|";
    file << contacts[numberOfUserContacts].idUser << "|";
    file << contacts[numberOfUserContacts].name << "|";
    file << contacts[numberOfUserContacts].surname << "|";
    file << contacts[numberOfUserContacts].phone << "|";
    file << contacts[numberOfUserContacts].address << "|";
    file << contacts[numberOfUserContacts].email << endl;
    file.close();
}

int checkHowManyContacts ()
{
    fstream file;
    file.open("Recipients.txt", ios::in);

    if (file.good() == false)
    {
        return 0;
    }

    string line;
    int numberOfLines=0;
    while(!file.eof())
    {
        getline(file,line);
        if (line != "")
        {
            numberOfLines++;
        }
    }
    file.close();
    return numberOfLines;
}

int checkIDofTheLastContact()
{
    int numberOfAllContacts = checkIDofTheLastContact();
    if (numberOfAllContacts == 0)
    {
        return 0;
    }
    else
    {
        fstream file;
        string line;
        file.open("Recipients.txt", ios::in);
        for (int i=0; i < numberOfAllContacts; i++ )
        {
            if (i == numberOfAllContacts-1)
            {
                getline(file,line,'|');
                file.close();
                return atoi(line.c_str());
            }
            else
            {
                getline(file,line);
            }
        }
    }
}

void addContact (vector<Contact> &contacts, int &numberOfUserContacts, int idOfLoggedUser)
{
    string name, surname, phone, address, email;
    cout << "Enter name: ";
    cin >> name;
    cout << "Enter surname: ";
    cin >> surname;
    cout << "Enter phone: ";
    cin.sync();
    getline(cin,phone);
    cout << "Enter address e-mail: ";
    cin.sync();
    cin >> email;
    cout << "Enter address: ";
    cin.sync();
    getline(cin,address);

    contacts.push_back(Contact());
    int numberOfAllContacts = checkHowManyContacts();
    if (numberOfAllContacts == 0)
    {
        contacts[numberOfUserContacts].idRecipient = 1;
    }
    else
    {
        contacts[numberOfUserContacts].idRecipient = checkIDofTheLastContact()+1;
    }
    contacts[numberOfUserContacts].idUser=idOfLoggedUser;
    contacts[numberOfUserContacts].name=changeFirstLetter(name);
    contacts[numberOfUserContacts].surname=changeFirstLetter(surname);
    contacts[numberOfUserContacts].phone=phone;
    contacts[numberOfUserContacts].email=email;
    contacts[numberOfUserContacts].address=address;

    addContactToFile(contacts, numberOfUserContacts);
    numberOfUserContacts++;

    cout << endl << "Contact has been added to the address book." << endl;
    Sleep(2000);
}

int readContactsFromFile (vector<Contact> &contacts, int idOfLoggedUser)
{
    fstream file;
    file.open("Recipients.txt", ios::in);
    if (file.good() == false)
    {
        return 0;
    }
    string data,idContact,idUser;
    int numbrOfContacts=0;

    while (!file.eof())
    {
        getline(file,idContact,'|');
        if (idContact != "")
        {
            getline(file,idUser,'|');
            if(atoi(idUser.c_str()) == idOfLoggedUser)
            {
                contacts.push_back(Contact());
                contacts[numbrOfContacts].idRecipient = atoi(idContact.c_str());
                contacts[numbrOfContacts].idUser = atoi(idUser.c_str());
                getline(file,data,'|');
                contacts[numbrOfContacts].name = data;
                getline(file,data,'|');
                contacts[numbrOfContacts].surname = data;
                getline(file,data,'|');
                contacts[numbrOfContacts].phone = data;
                getline(file,data,'|');
                contacts[numbrOfContacts].email = data;
                getline(file,data);
                contacts[numbrOfContacts].address = data;
                numbrOfContacts++;
            }
            else
            {
                getline(file,data,'|');
                getline(file,data,'|');
                getline(file,data,'|');
                getline(file,data,'|');
                getline(file,data);
            }
        }
    }
    file.close();
    return numbrOfContacts;
}

void moveToAddressBook(vector <User> &users, int &numberOfUsers, int idOfLoggedUser)
{
    vector<Contact> contacts;
    char choice;

    int numberOfUserContacts = readContactsFromFile(contacts, idOfLoggedUser);

    while (1)
    {
        system ("cls");
        cout << "1. Add new Contact" << endl;
        cout << "2. Seatch for contact by name" << endl;
        cout << "3. Seatch for contact by surname" << endl;
        cout << "4. Display all contacts" << endl;
        cout << "5. Delete Contact" << endl;
        cout << "6. Edit Contact" << endl;
        cout << "7. Change password" << endl;
        cout << "8. Log out" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (!(choice == '1' || choice == '8' || choice == '7') &&  numberOfUserContacts == 0)
        {
            cout << "Address book is empty!" << endl << "Add file \"Recipients.txt\" or new contacts."<<endl;
            Sleep (4000);
        }
        else if (choice == '1')
        {
            system ("cls");
            addContact(contacts, numberOfUserContacts, idOfLoggedUser);
        }

        else if (choice == '2')
        {
            system ("cls");
            searchByName(contacts);
        }

        else if (choice == '3')
        {
            system ("cls");
            searchBySurname(contacts);
        }
        else if (choice== '4')
        {
            system ("cls");
            displayAllContacts(contacts);
        }
        else if (choice== '5')
        {
            system ("cls");
            deleteContact(contacts, numberOfUserContacts);
        }
        else if (choice == '6')
        {
            system ("cls");
            editContact(contacts);
        }
        else if (choice == '7')
        {
            changePassword(users, numberOfUsers, idOfLoggedUser);
        }
        else if (choice == '8')
        {
            idOfLoggedUser=0;
            break;
        }
    }
}
