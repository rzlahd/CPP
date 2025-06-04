#include <stdio.h> // Required for standard input/output functions like printf, scanf, fopen, fclose
#include <stdlib.h> // Required for general utilities like exit, malloc, free
#include <string.h> // Required for string manipulation functions like strcmp

// Function to display the content of a file
void readFileContent(const char *filename) {
    FILE *file; // Declare a file pointer
    char ch;    // Character to read from the file

    // Open the file in read mode ("r")
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("Error: Could not open file '%s' for reading.\n", filename);
        return; // Exit the function if file opening failed
    }

    printf("\n--- Content of '%s' ---\n", filename);
    // Read character by character until the end of the file (EOF)
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch); // Print each character to the console
    }
    printf("\n--- End of file ---\n");

    // Close the file
    fclose(file);
}

// Function to write content to a file (effectively 'Save' or 'Save As')
void writeToFile(const char *filename) {
    FILE *file; // Declare a file pointer
    char content[1000]; // Buffer to store user input (max 999 characters + null terminator)

    // Open the file in write mode ("w").
    // If the file exists, its content will be truncated (overwritten).
    // If the file does not exist, it will be created.
    file = fopen(filename, "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return; // Exit the function if file opening failed
    }

    printf("\nEnter content to write to '%s' (type 'END' on a new line to finish):\n", filename);
    printf("------------------------------------------------------------------\n");

    // Read lines from standard input until "END" is entered
    while (fgets(content, sizeof(content), stdin) != NULL) {
        // Check if a newline was read. If not, it means the line was longer than the buffer.
        // In that case, we need to clear the rest of the input buffer to prevent issues with subsequent fgets calls.
        if (strchr(content, '\n') == NULL && strlen(content) == sizeof(content) - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Consume remaining characters in the line
        }

        // Remove the newline character if present (fgets includes it)
        content[strcspn(content, "\n")] = 0;

        // Check if the user typed "END" to finish input
        if (strcmp(content, "END") == 0) {
            break; // Exit the loop
        }

        // Write the content to the file, followed by a newline for readability
        fprintf(file, "%s\n", content);
    }

    printf("------------------------------------------------------------------\n");
    printf("Content successfully written to '%s'.\n", filename);

    // Close the file
    fclose(file);
}

// Main function where the program execution begins
int main() {
    char filename[256]; // Buffer to store the filename (max 255 characters + null terminator)
    int choice;         // Variable to store user's menu choice

    printf("Welcome to the C Console Notepad!\n");

    while (1) { // Infinite loop for the menu
        printf("\n--- Menu ---\n");
        printf("1. Read from a file\n");
        printf("2. Write/Save to a file (creates/overwrites)\n"); // Updated menu option
        printf("3. Exit\n");
        printf("Enter your choice: ");

        // Read user's choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer to prevent infinite loops on bad input
            while (getchar() != '\n');
            continue; // Go back to the beginning of the loop
        }

        // Consume the newline character left by scanf
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("Enter the filename to read from: ");
                // Read the filename from user input
                if (fgets(filename, sizeof(filename), stdin) != NULL) {
                    // Remove the newline character from the filename
                    filename[strcspn(filename, "\n")] = 0;
                    readFileContent(filename); // Call the read function
                }
                break;
            case 2:
                printf("Enter the filename to write to: ");
                // Read the filename from user input
                if (fgets(filename, sizeof(filename), stdin) != NULL) {
                    // Remove the newline character from the filename
                    filename[strcspn(filename, "\n")] = 0;
                    writeToFile(filename); // Call the write function
                }
                break;
            case 3:
                printf("Exiting C Console Notepad. Goodbye!\n");
                exit(0); // Exit the program
            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                break;
        }
    }

    return 0; // Indicate successful program execution (though exit(0) handles this for case 3)
}
