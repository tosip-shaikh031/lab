#1.3 Create an address book program using the bourne-again shell. It should use functions to perform the 
#required tasks. It should be menu-based, allowing you the options of: 
#1. Search address book 
#2. Add entries 
#3. Remove / edit entries. 
#You will also need a "display" function to display a record or records when selected. 
 
#Suggested record format: Use colons to separate fields.

#-------------------------------------------------------------------------------------------------------
#!/bin/bash
# ---------------------------------------------------
# Address Book Program (Menu-driven, Bash)
# File format: Name:Phone:Email:Address
# ---------------------------------------------------

BOOK="addressbook.txt"

# Create file if not exists
if [ ! -f "$BOOK" ]; then
    touch "$BOOK"
fi

# ---------------------------------------------------
# Function 1: Add New Entry
# ---------------------------------------------------
add_entry() {
    read -p "Enter Name: " name
    read -p "Enter Phone (10 digits): " phone
    read -p "Enter Email: " email
    read -p "Enter Address: " address

    # Trim leading/trailing spaces
    name=$(echo "$name" | xargs)
    phone=$(echo "$phone" | xargs)
    email=$(echo "$email" | xargs)
    address=$(echo "$address" | xargs)

    # Validation checks
    if [ -z "$name" ] || [ -z "$phone" ] || [ -z "$email" ] || [ -z "$address" ]; then
        echo "Error: All fields are required."
        return
    fi

    if ! [[ "$name" =~ ^[A-Za-z\ ]+$ ]]; then
        echo "Error: Name must contain only letters and spaces."
        return
    fi

    if ! [[ "$phone" =~ ^[0-9]{10}$ ]]; then
        echo "Error: Phone number must be exactly 10 digits."
        return
    fi

    if ! [[ "$email" =~ ^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$ ]]; then
        echo "Error: Invalid email format."
        return
    fi

    if grep -q "^$name:" "$BOOK"; then
        echo "Error: Entry for '$name' already exists."
        return
    fi

    # Append record
    echo "${name}:${phone}:${email}:${address}" >> "$BOOK"
    echo "Record added successfully."
}

# ---------------------------------------------------
# Function 2: Search Entry
# ---------------------------------------------------
search_entry() {
    read -p "Enter name, phone, or email to search: " key
    echo "Search Results:"
    grep -i "$key" "$BOOK" | awk -F":" '{ printf "Name: %-15s | Phone: %-10s | Email: %-20s | Address: %s\n", $1, $2, $3, $4 }' \
        || echo "No matching record found."
}

# ---------------------------------------------------
# Function 3: Display All Records
# ---------------------------------------------------
display_all() {
    if [ ! -s "$BOOK" ]; then
        echo "Address book is empty."
        return
    fi

    echo "------------------- ADDRESS BOOK -------------------"
    awk -F":" '{ printf "Name: %-15s | Phone: %-10s | Email: %-20s | Address: %s\n", $1, $2, $3, $4 }' "$BOOK"
    echo "----------------------------------------------------"
}

# ---------------------------------------------------
# Function 4: Edit Entry
# ---------------------------------------------------
edit_entry() {
    read -p "Enter the name of the entry to edit: " name

    if ! grep -q "^$name:" "$BOOK"; then
        echo "No entry found with that name."
        return
    fi

    old_record=$(grep "^$name:" "$BOOK")
    echo "Editing record: $old_record"

    read -p "Enter new Phone: " new_phone
    read -p "Enter new Email : " new_email
    read -p "Enter new Address : " new_address

    new_phone=${new_phone:-$old_phone}
    new_email=${new_email:-$old_email}
    new_address=${new_address:-$old_address}

    # Validations for edited values
    if ! [[ "$new_phone" =~ ^[0-9]{10}$ ]]; then
        echo "Error: Invalid phone number."
        return
    fi

    if ! [[ "$new_email" =~ ^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$ ]]; then
        echo "Error: Invalid email format."
        return
    fi

    # Replace record
    grep -v "^$name:" "$BOOK" > temp.txt
    echo "${name}:${new_phone}:${new_email}:${new_address}" >> temp.txt
    mv temp.txt "$BOOK"
    echo "Record updated successfully."
}

# ---------------------------------------------------
# Function 5: Delete Entry
# ---------------------------------------------------
delete_entry() {
    read -p "Enter the name of the entry to delete: " name

    if grep -q "^$name:" "$BOOK"; then
        grep -v "^$name:" "$BOOK" > temp.txt && mv temp.txt "$BOOK"
        echo "Record deleted successfully."
    else
        echo "No entry found with that name."
    fi
}

# ---------------------------------------------------
# Function 6: Menu
# ---------------------------------------------------
menu() {
    while true
    do
        echo ""
        echo "======================================="
        echo "          ADDRESS BOOK MENU"
        echo "======================================="
        echo "1. Add New Entry"
        echo "2. Search Entry"
        echo "3. Display All Entries"
        echo "4. Edit Entry"
        echo "5. Delete Entry"
        echo "6. Exit"
        echo "======================================="
        read -p "Enter your choice [1-6]: " choice

        case $choice in
            1) add_entry ;;
            2) search_entry ;;
            3) display_all ;;
            4) edit_entry ;;
            5) delete_entry ;;
            6) echo "Exiting..."; break ;;
            *) echo "Invalid choice. Try again." ;;
        esac
    done
}

# ---------------------------------------------------
# Start Program
# ---------------------------------------------------
menu
