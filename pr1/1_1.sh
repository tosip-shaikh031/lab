
#Write a shell script to implement an address book (address.txt) that contains ID, Name, and Phone with
#the following functions:

#1. Search Address Book
#2. Add an address book entry
#3. Remove an address book entry
#4. Quit the program.
#Suggested record format: Use semi-colon (;) to separate fields.


#---------------------------------------------------------------------------------------------------------------

#!/bin/bash
# Address Book Script (Function-Based)
# File: address.sh

BOOK="address.txt"

# Create file if not exists
if [ ! -f "$BOOK" ]; then
    touch "$BOOK"
fi

# -------------------------------
# Function: Search for an entry
# -------------------------------
search_entry() {
    read -p "Enter name or ID to search: " key
    echo "Search Results:"
    grep -i "$key" "$BOOK" || echo "No matching record found."
}

# -------------------------------
# Function: Add new entry
# -------------------------------
add_entry() {
    read -p "Enter ID: " id
    read -p "Enter Name: " name
    read -p "Enter Phone: " phone

    # Check if ID already exists
    if grep -q "^$id;" "$BOOK"; then
        echo "Error: ID already exists. Try again with a new ID."
    else
        echo "$id;$name;$phone" >> "$BOOK"
        echo "Record added successfully."
    fi
}
# -------------------------------
# Function: Remove an entry
# -------------------------------
remove_entry() {
    read -p "Enter ID to remove: " rid
    if grep -q "^$rid;" "$BOOK"; then
        grep -v "^$rid;" "$BOOK" > temp.txt && mv temp.txt "$BOOK"
        echo "Record removed successfully."
    else
        echo "No such ID found."
    fi
}

# -------------------------------
# Function: Display all entries
# -------------------------------
show_all() {
    echo "-----------------------------"
    echo "   ID    |   Name   |  Phone"
    echo "-----------------------------"
    awk -F";" '{ printf "%-8s | %-8s | %-10s\n", $1, $2, $3 }' "$BOOK"
    echo "-----------------------------"
}

# -------------------------------
# Main Menu Function
# -------------------------------
menu() {
    while true
    do
        echo ""
        echo "=============================="
        echo " Address Book Menu"
        echo "=============================="
        echo "1. Show All Entries"
        echo "2. Search Entry"
        echo "3. Add Entry"
        echo "4. Remove Entry"
        echo "5. Quit"
        echo "=============================="
        read -p "Enter your choice [1-5]: " choice

        case $choice in
            1) show_all ;;
            2) search_entry ;;
            3) add_entry ;;
            4) remove_entry ;;
            5) echo "Exiting..."; break ;;
            *) echo "Invalid choice. Try again." ;;
        esac
    done
}

# Start the menu
menu
