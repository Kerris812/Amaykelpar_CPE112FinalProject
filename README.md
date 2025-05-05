✅ Amaykelpar To-Do List Manager


## 📚 Project Info

This is a team project developed as part of **CPE112 - Programming with Data Structure**, **Department of Computer Engineering, King Mongkut's University of Technology Thonburi**.

**Team Name:** Amaykelpar  
**Team Members:**
1. 67070503479_Sai Bhone Myint Myat
2. 67070503486_Thu Thu Wai
3. 67070503482_Moe Htet Aung

---

## 📌 Features

- 📂 **Categorized Tasks**  – Organize tasks by categories.
- 🔄 **Priority Queue**     – Automatically order tasks by priority and due date.
- 📅 **Deadline**           – Add, update, and display deadlines with real-time countdowns.
- 💾 **Storage**            – Tasks are saved and loaded via CSV files.
- ✅ **Mark Status**        – Track tasks as pending, in-progress, or completed.
- 📊 **Live Statistics**    – View productivity stats with color-coded breakdowns.
- 🗂️ **Advanced Views**     – Filter tasks by status, category, time left, or completion.
- ♻️ **Restore Tasks**      – Soft-deletion system with recovery stack.
- 🌈 **Terminal UI**        – Clean, aesthetic output using box-drawing characters and ANSI color.

---

### 1. How to Run the Program
```bash
gcc main.c task_manager.c priority_queue.c file_handler.c ui.c utils.c -o todo
./todo
```

> Tasks will be stored in a file named `tasks.csv` automatically after every action.

---

## 📁 Project Structure

File                Description                                 
`main.c`            Entry point with interactive menu system    
`task_manager.c/h`  All core task and category logic            
`priority_queue.c/h`Custom min-heap implementation for sorting  
`file_handler.c/h`  Load/save task data from/to CSV             
`ui.c/h`            Dynamic and colored terminal UI             
`utils.c`           Input helpers, time/date validators         

---

## 🧠 Data Structures Used

Priority Queue
 - Used for automatically sorting tasks by priority and due date. Implemented with a min-heap.

Circular Linked List
 - Used to sort and manage task names alphabetically (A → Z) for displaying in sorted views.

Stack
 - Used to manage and restore deleted tasks (last-in, first-out logic).

Singly Linked List
 - Used to store and manage task categories as a dynamically growing list.

Array
 - Used for temporary storage and rendering of tasks when displaying sorted task tables.

---


## 📦 Saving Format (CSV)

`tasks.csv` and saves each task as:
```
<CategoryName>,<ID>,<Title>,<Description>,<Priority>,<DueDate>,<DueTime>,<Status>
```


> Goodyyyy.


