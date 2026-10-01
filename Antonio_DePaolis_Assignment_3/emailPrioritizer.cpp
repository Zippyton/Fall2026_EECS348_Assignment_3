/*
Program Name: Email Prioritizer
Description: Prioritizes emails inputted via a text file by putting them in a MaxHeap
             then produces outputs for each command inside the text file until it reaches the end using C++
Inputs: Text file containing email data
Outputs: Outputs what each command does during file reading.
Collaborators: Gemini for whole code
Other soruces: geeksforgeek: (Leap Year) https://www.geeksforgeeks.org/dsa/program-check-given-year-leap-year/
Author: Antonio DePaolis
Creation Date: 9/30/2026
Revision Date: 9/30/2026
Revisions: Modified and added comments. Modified the trim_whitespace() function to handle leading whitespace.
*/

#include <iostream>	// Include iostream for standard input and output operations
#include <fstream>	// Include fstream for reading from files
#include <sstream>	// Include sstream for string stream manipulation
#include <string>	// Include string for using std::string objects
#include <vector>	// Include vector for dynamic array storage

bool isValidDate(int m, int d, int y) {	// Function that checks if a month, day, and year are a valid date
    if (m < 1 || m > 12 || d < 1 || d > 31) return false;	// Return false if month or day values are outside of standard bounds

    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	// Array storing the maximum number of days for each month 
    bool isLeap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);	// Determine if the year is a leap year
    if (isLeap) daysInMonth[2] = 29;	// if a leap year change february to 29

    return d <= daysInMonth[m];	// Return true if day fits the days in months array for its current month
}	// End of isValidDate function

struct Email {	// Structure for an email record
    std::string category;	// Stores the sender category (Boss, Subordinate, etc...)
    std::string subject;	// Stores the subject line of the email
    std::string date;	// Stores the date string in MM-DD-YYYY format
    int priorityScore;	// Priority score used to determine heap ordering stored as an int
    int year, month, day;	// the date components to determine priority
    bool validDate; // Flag to check if date is valid

    Email() : priorityScore(0), year(0), month(0), day(0) {}	// Default constructor initializing numeric fields to zero

    Email(const std::string& cat, const std::string& subj, const std::string& d)	// Parameterized constructor to initialize email data
        : category(cat), subject(subj), date(d) {	// Initialize member variables with constructor parameters
        
        if (category == "Boss") priorityScore = 5;	// Assign highest priority score of 5 to Boss emails
        else if (category == "Subordinate") priorityScore = 4;	// Assign priority score of 4 to Subordinate emails
        else if (category == "Peer") priorityScore = 3;	// Assign priority score of 3 to Peer emails
        else if (category == "ImportantPerson") priorityScore = 2;	// Assign priority score of 2 to ImportantPerson emails
        else priorityScore = 1;	// Assign default priority score of 1 to all other senders

        std::stringstream ss(date);	// Create a string stream to parse the date string
        char dash1, dash2;	// Characters to capture hyphen delimiters in the date string
        int m = 0, dayVal = 0, y = 0;	// Temporary variables to store month, day, and year

        if (ss >> m >> dash1 >> dayVal >> dash2 >> y && isValidDate(m, dayVal, y)) {	// Parse date components and check if date is valid
            month = m;	// Assign parsed month if valid
            day = dayVal;   // Assign parsed day if valid
            year = y;	// Assign parsed year if valid
            validDate = true;    // Marks date as valid
        } else {	// if date is invalid or an erorr occurs marks it as invalid
            validDate = false;    // Marks date as invalid
        }	// End of date validation conditional block
    }	// End of Email constructor

    bool isHigherPriorityThan(const Email& other) const {	// Member function to check if given email has higher priority over another email
        if (this->priorityScore != other.priorityScore) {	// Compares category priority scores first
            return this->priorityScore > other.priorityScore;	// Return true if this email has a higher priority score
        }	// End of priority score comparison
        if (this->year != other.year) return this->year > other.year;	// Break tie using the year (newer year is higher priority)
        if (this->month != other.month) return this->month > other.month;	// Break tie using the month (newer month is higher priority)
        return this->day > other.day;	// Break tie using the day (newer day is higher priority)
    }	// End of isHigherPriorityThan function
};	// End of Email struct definition

class MaxHeap {	// Class implementation of a Max-Heap priority queue for Emails
private:	// Private data members and helper methods
    std::vector<Email> heap;	// Internal vector to store heap elements

    void heapifyUp(int index) { // Helper function to restore max-heap property upwards from given index
        while (index > 0) {	// Continue climbing up until reaching the root
            int parentIndex = (index - 1) / 2;	// Calculate the parent node index
            if (heap[index].isHigherPriorityThan(heap[parentIndex])) {	// Check if current node has a bigger priority than parent node
                std::swap(heap[index], heap[parentIndex]);	// Swap current node with parent node
                index = parentIndex;	// Move tracking index up to parent position
            } else {	// Max-heap property is satisfied
                break;	// Terminate loop early
            }	// End of conditional check
        }	// End of while loop
    }	// End of heapifyUp function

    void heapifyDown(int index) {	// Helper function to restore max-heap property downwards from given index
        int size = static_cast<int>(heap.size());	// Get total number of elements in the heap
        while (index < size) {	// Continue down as long as index is within bounds
            int leftChild = 2 * index + 1;	// Calculate left child index
            int rightChild = 2 * index + 2;	// Calculate right child index
            int largest = index;	// Assume current node is the largest/highest priority

            if (leftChild < size && heap[leftChild].isHigherPriorityThan(heap[largest])) {	// Check if left child exists and has 
                                                                                            // a bigger priority current largest
                largest = leftChild;	// Update largest index to left child
            }	// End of left child check
            if (rightChild < size && heap[rightChild].isHigherPriorityThan(heap[largest])) {	// Check if right child exists and has
                                                                                                // a bigger priority current largest
                largest = rightChild;	// Update largest index to right child
            }	// End of right child check

            if (largest != index) {	// Checks if a child node had higher priority if largest is no longer the same as the index
                std::swap(heap[index], heap[largest]);	// Swap current node with the higher priority child
                index = largest;	// Move tracking index down to child position
            } else {	// Max-heap property is satisfied
                break;	// Terminate loop early
            }	// End conditional check
        }	// End of while loop
    }	// End of heapifyDown function

public:	// Public interface methods
    void push(const Email& email) {	// Method to insert a new email into the max-heap
        heap.push_back(email);	// Add new email to the end of the vector
        heapifyUp(static_cast<int>(heap.size()) - 1);	// Bubble up the new element to restore heap order
    }	// End push function

    bool peek(Email& topEmail) const {	// Method that retrieves the highest priority email without removing it
        if (heap.empty()) return false;	// Return false if heap has no elements
        topEmail = heap[0];	// Copy root element into target parameter
        return true;	// Return true indicating successful peek
    }	// End of peek function

    bool pop() {	// Method that removes the highest priority email from the heap
        if (heap.empty()) return false;	// Return false if heap is empty
        heap[0] = heap.back();	// Overwrite root with the last element in vector
        heap.pop_back();	// Remove last element from vector
        heapifyDown(0);	// Bubble down new root element to restore heap order
        return true;	// Return true indicating successful removal
    }	// End of pop function

    int size() const {	// Method that gets the current number of emails in heap
        return static_cast<int>(heap.size());	// Return heap size as an integer
    }	// End of size function

    bool empty() const {	// Method to check if heap is empty
        return heap.empty();	// Return true if vector contains zero elements
    }	// End empty function
};	// End of MaxHeap class definition

std::string trim(const std::string& str) {	// Helper function that strips leading/trailing whitespace
    size_t first = str.find_first_not_of(" \t\r\n");	// Find index of first non-whitespace character
    if (first == std::string::npos) return "";	// Returns an empty string if all characters are whitespace
    size_t last = str.find_last_not_of(" \t\r\n");	// Find index of last non-whitespace character
    return str.substr(first, (last - first + 1));	// Extract and return the trimmed substring
}	// End of trim function

int main() {	// Main entry point of the program
    std::string filename;	// Variable to store the input command file name
    std::cout << "Enter command file name: ";	// Prompts the user for input file name
    std::cin >> filename;	// Reads the file name from standard input

    std::ifstream infile(filename);	// Open input file stream with given file name
    
    if (!infile.is_open()) {	// Edge case that Checks if file failed to open
        std::cerr << "Error: Unable to open file '" << filename << "'." << std::endl;	// Display error message to standard error stream
        return 1;	// Exit program with non-zero status indicating failure
    }	// End file check conditional

    MaxHeap priorityQueue;	// Initializes custom MaxHeap priority queue
    std::string line;	// Variable to store each line read from file

    while (std::getline(infile, line)) {	// Reads the input file line by line
        line = trim(line);	// Strips the whitespace from the line
        if (line.empty()) continue;	// Skips the line if it is empty after trimming

        if (line.rfind("EMAIL", 0) == 0) {	// Checks if the line begins with "EMAIL"
            std::string data = line.substr(5);	// Strip leading "EMAIL" keyword from line
            std::stringstream ss(data);	// Load remaining comma-separated data into stringstream
            std::string category, subject, date;	// Variables to hold parsed email field strings

            if (std::getline(ss, category, ',') &&	// Parse sender category up to first comma
                std::getline(ss, subject, ',') &&	// Parse email subject up to second comma
                std::getline(ss, date, ',')) {	// Parse date string up to third comma

                category = trim(category);	// Trim the whitespace from sender category
                subject = trim(subject);	// Trim the whitespace from email subject
                date = trim(date);	// Trim the whitespace from date string

                Email mail(category, subject, date);	// Construct Email object with parsed strings
                if (mail.validDate) {	// Checks if email has a valid date before inserting into queue
                    priorityQueue.push(mail);	// Insert the valid Email object into priority queue
                }	// Ignore the email completely if validDate is false
            }	// End of parsing check
        } 	// End of EMAIL command check
        else if (line == "NEXT") {	// Check if the command line is "NEXT"
            Email nextEmail;	// Variable to store top email copy
            if (priorityQueue.peek(nextEmail)) {	// Safely retrieve highest priority email if queue is not empty
                std::cout << "Next email:\n";	// Output section header for next email
                std::cout << "Sender: " << nextEmail.category << "\n";	// Prints the sender category
                std::cout << "Subject: " << nextEmail.subject << "\n";	// Prints the email subject
                std::cout << "Date: " << nextEmail.date << "\n\n";	// Prints the email date followed by an empty line
            } else {	// if queue is empty prints out a statment
                std::cout << "No emails in queue.\n\n";	// Output empty queue message
            }	// End of peek check
        } 	// End of NEXT command check
        else if (line == "READ") {	// Check if the command line is "READ"
            if (!priorityQueue.pop()) {	// Attempts to remove top email from priority queue
                std::cout << "No emails to read.\n\n";	// Output error message if queue was empty
            }	// End of pop check
        } 	// End of READ command check
        else if (line == "COUNT") {	// Check if the command line is "COUNT"
            std::cout << "There are " << priorityQueue.size() << " emails to be read.\n\n";	// Prints the total email count in queue
        }	// End of COUNT command check
    }	// End of file reading loop

    infile.close();	// Closes the input file stream
    return 0;	// Exits the program cleanly with return code 0
}	// End of main function