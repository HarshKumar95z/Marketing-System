#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

class Bill
{
private:
    string item;
    int rate;
    int quantity;

public:
    Bill() : item(""), rate(0), quantity(0) {}

    void setItem(const string &itemName) { item = itemName; }
    void setRate(int itemRate) { rate = itemRate; }
    void setQuantity(int itemQuantity) { quantity = itemQuantity; }

    string getItem() const { return item; }
    int getRate() const { return rate; }
    int getQuantity() const { return quantity; }

    string format() const
    {
        stringstream ss;
        ss << item << " : " << rate << " : " << quantity;
        return ss.str();
    }
};

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
}

void addItem()
{
    clearScreen();
    ofstream outFile("Bill.txt", ios::app);
    if (!outFile)
    {
        cerr << "Error: Unable to open file for writing!" << endl;
        pause();
        return;
    }

    string itemName;
    int rate, quantity;

    cout << "Enter Item Name: ";
    cin.ignore();
    getline(cin, itemName);
    cout << "Enter Rate of Item: ";
    cin >> rate;
    cout << "Enter Quantity of Item: ";
    cin >> quantity;

    Bill newBill;
    newBill.setItem(itemName);
    newBill.setRate(rate);
    newBill.setQuantity(quantity);

    outFile << newBill.format() << endl;
    outFile.close();

    cout << "Item added successfully!" << endl;
    pause();
}

void printBill()
{
    clearScreen();
    int totalAmount = 0;

    while (true)
    {
        clearScreen();
        int choice;
        cout << "1. Add to Bill" << endl;
        cout << "2. Finalize and Show Total Bill" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string itemName;
            int quantity;

            cout << "Enter Item Name: ";
            cin.ignore(); // Clear buffer
            getline(cin, itemName); // Handle multi-word names
            cout << "Enter Quantity: ";
            cin >> quantity;

            ifstream inFile("Bill.txt");
            ofstream tempFile("Bill_temp.txt");
            if (!inFile || !tempFile)
            {
                cerr << "Error: Unable to access files!" << endl;
                pause();
                return;
            }

            string line;
            bool itemFound = false;

            while (getline(inFile, line))
            {
                stringstream ss(line);
                string fileItem;
                int fileRate, fileQuantity;
                char delimiter;

                getline(ss, fileItem, ':'); // Extract item name
                fileItem = fileItem.substr(0, fileItem.find_last_not_of(" \t") + 1); // Trim whitespace
                ss >> fileRate >> delimiter >> fileQuantity;

                if (fileItem == itemName)
                {
                    itemFound = true;

                    if (quantity <= fileQuantity)
                    {
                        int amount = fileRate * quantity;
                        totalAmount += amount;

                        cout << setw(15) << "Item" << setw(10) << "Rate" << setw(10) << "Quantity" << setw(10) << "Amount" << endl;
                        cout << setw(15) << fileItem << setw(10) << fileRate << setw(10) << quantity << setw(10) << amount << endl;

                        fileQuantity -= quantity;

                        if (fileQuantity > 0)
                        {
                            tempFile << fileItem << " : " << fileRate << " : " << fileQuantity << endl;
                        }
                    }
                    else
                    {
                        cout << "Insufficient quantity for item: " << fileItem << endl;
                        tempFile << line << endl; // Write the original line
                    }
                }
                else
                {
                    tempFile << line << endl;
                }
            }

            if (!itemFound)
            {
                cout << "Item not found in inventory!" << endl;
            }

            inFile.close();
            tempFile.close();

            // Replace the original file with the updated file
            if (remove("Bill.txt") != 0 || rename("Bill_temp.txt", "Bill.txt") != 0)
            {
                cerr << "Error: Unable to update the inventory file!" << endl;
            }

            pause();
        }
        else if (choice == 2)
        {
            cout << "Total Amount: " << totalAmount << endl;
            pause();
            return;
        }
        else
        {
            cout << "Invalid choice. Try again." << endl;
            pause();
        }
    }
}



int main()
{
    while (true)
    {
        clearScreen();
        cout << "Market Billing System" << endl;
        cout << "1. Add Item to Inventory" << endl;
        cout << "2. Generate Bill" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
            addItem();
            break;
        case 2:
            printBill();
            break;
        case 3:
            cout << "Thank you for using the system. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Try again." << endl;
            pause();
        }
    }
}
