#Write a script program to create a phonebook. Using the phonebook, perform the following tasks:

#1. Add a new name to a phone book file
#2. Display all matches to a name or phone number
#3. Sort the phone book by the last name
#4. Delete an entry
#Suggested record format: Use tabs to separate fields.


#--------------------------------------------------------------------------------------------------

#!/bin/bash
# -----------------------------
# Simple Phonebook Script with Validations
# -----------------------------
# File format: FirstName<tab>LastName<tab>PhoneNumber
# -----------------------------

BOOK="phonebook.txt"

# Create file if not exists
if [ ! -f "$BOOK" ]; then
    touch "$BOOK"
fi

# -----------------------------
# Function 1: Add New Entry
# -----------------------------
add_entry() {
    echo "Enter First Name:"
    read fname
    echo "Enter Last Name:"
    read lname
    echo "Enter Phone Number:"
    read phone

    # Check for empty fields
    if [ -z "$fname" ] || [ -z "$lname" ] || [ -z "$phone" ]; then
        echo "Error: All fields are required."
        return
    fi

    # Validate first name (alphabets only)
    if [[ ! "$fname" =~ ^[A-Za-z]+$ ]]; then
        echo "Error: First name should contain only alphabets."
        return
    fi

    # Validate last name (alphabets only)
    if [[ ! "$lname" =~ ^[A-Za-z]+$ ]]; then
        echo "Error: Last name should contain only alphabets."
        return
    fi

    # Validate phone number (exactly 10 digits)
    if [[ ! "$phone" =~ ^[0-9]{10}$ ]]; then
        echo "Error: Phone number must be exactly 10 digits."
        return
    fi

    # Check for duplicate phone number
    if grep -q "$phone" "$BOOK"; then
        echo "Error: This phone number already exists."
        return
    fi

    # Append new record
    echo -e "${fname}\t${lname}\t${phone}" >> "$BOOK"
    echo "Record added successfully."
}

# -----------------------------
# Function 2: Search Entry
# -----------------------------
search_entry() {
    echo "Enter name or phone number to search:"
    read key

    echo "Search Results:"
    grep -i "$key" "$BOOK" || echo "No matching record found."
}

# -----------------------------
# Function 3: Sort by Last Name
# -----------------------------
sort_entries() {
    echo "Phonebook sorted by last name:"
    sort -k2,2 "$BOOK" | column -t
}

# -----------------------------
# Function 4: Delete Entry
# -----------------------------
delete_entry() {
    echo "Enter phone number to delete:"
    read dphone

    if grep -q "$dphone" "$BOOK"; then
        grep -v "$dphone" "$BOOK" > temp.txt && mv temp.txt "$BOOK"
        echo "Record deleted successfully."
    else
        echo "No entry found with that phone number."
    fi
}

# -----------------------------
# Function 5: Display Menu
# -----------------------------
menu() {
    while true
    do
        echo ""
        echo "=============================="
        echo "       PHONEBOOK MENU"
        echo "=============================="
        echo "1. Add New Entry"
        echo "2. Search Entry"
        echo "3. Sort by Last Name"
        echo "4. Delete Entry"
        echo "5. Exit"
        echo "=============================="
        read -p "Enter your choice [1-5]: " choice

        case $choice in
            1) add_entry ;;
            2) search_entry ;;
            3) sort_entries ;;
            4) delete_entry ;;
            5) echo "Exiting..."; break ;;
            *) echo "Invalid choice. Try again." ;;
        esac
    done
}

# -----------------------------
# Start the Program
# -----------------------------
menu
