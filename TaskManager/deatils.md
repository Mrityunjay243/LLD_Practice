**Requirements**
- The task management system should allow the users to create, update, and delete tasks
- Each task should have a title, description, due date, and status
- Users should be able to assign tasks to other users and set reminders to the tasks 
- The system should support searching and filtering tasks based on various criteria 
- Users should be able to mark tasks as completed when they view their task history 

**Class, Interfaces, and Enums**
- The user class reporesent a user in the system, with properties such as id, name, email
- TaskStatus Enum - pending, in progress, completed
- Task class represents a task in the system, id, title, description, due date, priority, status, and assigned user etc 
- TaskManager is the core task management system, follows a singleton pattern 
- TaskManager uses concurrent data structures to handle concurrent access to tasks and to ensure thread safety 
- TaskManager class provides methods like creating, updating, deleting, and filtering tasks 
- main.cpp is out entry point to the demo
