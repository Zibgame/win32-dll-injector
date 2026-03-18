# ⚙️ Win32 DLL Injector

> 🚀 A low-level Windows DLL injection tool built in C/C++

---

## 👨‍💻 Author

**Zibrian Cadinot**

---

## 📌 Overview

This project demonstrates how to inject a DLL into a running Windows process using native Win32 APIs.

It is designed for learning:

* 🧠 Windows internals
* 🧬 Process memory manipulation
* 🧵 Remote thread creation
* ⚙️ Low-level system programming

---

## 🛠 Features

* 🔍 Open target process by PID
* 🧠 Allocate memory in remote process
* ✍️ Write DLL path into target memory
* 🧵 Create remote thread
* 📦 Load DLL via `LoadLibraryA`

---

## 📁 Project Structure

```
src/
  injector/      → DLL injector
  dll/           → Injected DLL
  target/        → Test program

bin/             → Executables & DLL
build/           → Object files
```

---

## ⚡ Build

Using `cl` (MSVC):

```
make
```

Make sure to run in:

```
Developer Command Prompt for Visual Studio
```

---

## ▶️ Usage

1. Run target program:

```
.\bin\main.exe
```

2. Get PID:

```
tasklist
```

3. Inject DLL:

```
.\bin\dll_injector.exe <PID> bin\dll.dll
```

---

## 🧠 How It Works

1. OpenProcess → get handle to target
2. VirtualAllocEx → allocate memory
3. WriteProcessMemory → write DLL path
4. CreateRemoteThread → call LoadLibraryA
5. DLL is loaded into target process

---

## ⚠️ Disclaimer

This project is for **educational purposes only**.

Do not use on software you do not own or have permission to analyze.

---

## ⭐ Why this project

* Learn real-world low-level programming
* Understand how memory works in Windows
* Build strong reverse engineering foundations

---

## 🚀 Future Improvements

* Manual mapping (no LoadLibrary)
* Stealth injection techniques
* Memory utilities (pattern scan, hooks)
* Lua integration

---

## 🧩 Skills Demonstrated

* C / C++
* Win32 API
* Memory management
* Debugging & reverse engineering mindset

---

## 💬 Contact

Feel free to reach out or contribute!

---

⭐ If you found this useful, consider starring the repo!
