#1.4 Create a file management program using the bourne-again shell. It should use functions to perform the 
#required tasks. It should be menu-based, allowing you the options of: 
#1. Test if file exists 
#2. Read a file 
#3. Delete a file 
#4. Display a list of files 
 
#e.g. book.txt and test.txt files are available in present working directory. The filename = “book.txt” 
#contents: 
#1. Pro AngularJS 
#2. Learning JQuery  
#3. PHP Programming

#----------------------------------------------------------------------------------------------------------------

#!/bin/bash
# ---------------------------------------------------
# File Management Program (Menu-driven)
# ---------------------------------------------------

# Function 1: Test if file exists
test_file() {
    read -p "Enter filename to test: " filename
    if [ -f "$filename" ]; then
        echo "File '$filename' exists."
    else
        echo "File '$filename' does NOT exist."
    fi
}

# Function 2: Read a file
read_file() {
    read -p "Enter filename to read: " filename
    if [ -f "$filename" ]; then
        echo "----- Contents of $filename -----"
        cat "$filename"
        echo "---------------------------------"
    else
        echo "Error: File '$filename' not found."
    fi
}

# Function 3: Delete a file
delete_file() {
    read -p "Enter filename to delete: " filename
    if [ -f "$filename" ]; then
        read -p "Are you sure you want to delete '$filename'? (y/n): " confirm
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            rm "$filename"
            echo "File '$filename' deleted successfully."
        else
            echo "Deletion cancelled."
        fi
    else
        echo "Error: File '$filename' not found."
    fi
}

# Function 4: Display list of files
list_files() {
    echo "----- List of Files in Current Directory -----"
    ls -lh | awk '{print $9, $5}' | sed '/^$/d'
    echo "----------------------------------------------"
}

# ---------------------------------------------------
# Menu Function
# ---------------------------------------------------
menu() {
    while true
    do
        echo ""
        echo "========================================"
        echo "        FILE MANAGEMENT PROGRAM"
        echo "========================================"
        echo "1. Test if File Exists"
        echo "2. Read a File"
        echo "3. Delete a File"
        echo "4. Display List of Files"
        echo "5. Exit"
        echo "========================================"
        read -p "Enter your choice [1-5]: " choice

        case $choice in
            1) test_file ;;
            2) read_file ;;
            3) delete_file ;;
            4) list_files ;;
            5) echo "Exiting..."; break ;;
            *) echo "Invalid choice. Please try again." ;;
        esac
    done
}

# ---------------------------------------------------
# Start Program
# ---------------------------------------------------
menu
