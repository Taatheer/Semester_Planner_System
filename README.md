Overview
This project is a C programming assignment aimed at developing a Student Semester Planner application. The application is designed to help students manage their academic activities by offering functionalities like course registration, activity tracking, and progress monitoring. The project is structured to demonstrate the use of Object-Oriented Programming (OOP) principles and file handling in C.

Features
User Roles
Administrator
Manage user accounts (students, program coordinators, program leaders).
Update user account statuses.
Student
Register and unregister for modules and events.
Track credit points and academic progress.
Request and manage activities.
Programme Coordinator
Plan and manage academic activities.
Assign and update core modules.
Programme Leader
Monitor student activities and progress.
Generate reports on student engagement.
Key Functionalities
Login System: Secure login for different user roles.
Activity Management: Register, unregister, and track activities and modules.
User Management: Admin functionalities for managing user roles and accounts.
Data Handling: All user and activity data is stored in text files.
Program Design
Pseudocode Overview
The project follows a structured program design, starting with pseudocode to outline the operations and logic before translating them into C code. This approach ensures clarity and facilitates debugging and future maintenance.

Functions
The project is built with modular programming practices, with each function handling specific tasks such as:

clearScreen(): Clears the console screen.
validateMenuChoice(): Validates user input for menu selections.
logIn(): Manages the login process for all user roles.
readUserFile(): Reads and processes user data from text files.
updateActivity(): Updates student activity records.
regAccount(): Handles the registration of new user accounts.
addESMod(), addTW(), addComp(), addCS(): Functions to add elective modules, workshops, competitions, and club/society modules respectively.
Coding and Testing
Programming Concepts
Modular Programming: The project is broken down into functions to enhance readability and reusability.
File Handling: Extensive use of file I/O for storing and retrieving user and activity data.
Data Structures: Use of structures to manage complex data like user details and student activities.
Validations
Input validations are implemented throughout the project to ensure data integrity and prevent errors.

Additional Features
Encryption: Basic password encryption using a hashing algorithm.
Random ID Generation: Unique ID generation for students and activities.
Installation
Clone the repository:
bash
Copy code
git clone https://github.com/yourusername/student-semester-planner.git
Compile the code using a C compiler:
bash
Copy code
gcc -o semester_planner main.c
Run the program:
bash
Copy code
./semester_planner
Usage
Admin: Manage users and update account information.
Student: Register for modules, track activities, and manage academic progress.
Program Coordinator: Add and update core modules and activities.
Program Leader: Monitor student progress and generate reports.
Conclusion
The Student Semester Planner application is designed to simplify the management of academic activities for students and administrators. The project highlights the effective use of C programming concepts such as file handling, modular programming, and basic encryption.
