# ⚙️ Win32 DLL Injector

> 🚀 A low-level Windows DLL injection tool built in C/C++ with GUI support (ImGui + DirectX11)

---

## 👨‍💻 Author

**Zibrian Cadinot**

---

## 📌 Overview

This project demonstrates how to inject a DLL into a running Windows process using native Win32 APIs.

It also includes a **GUI injector** built with ImGui for a better user experience.

---

## 🛠 Features

* 🔍 Open target process by PID
* 🧠 Allocate memory in remote process
* ✍️ Write DLL path into target memory
* 🧵 Create remote thread
* 📦 Load DLL via `LoadLibraryA`
* 🖥 GUI interface (ImGui + DX11)

---

## 📁 Project Structure

```
src/
  injector/      → DLL injector logic
  dll/           → Injected DLL
  target/        → Test program
  imgui/         → ImGui source
  main.cpp       → GUI injector

bin/             → Output binaries
```

---

## ⚡ Build

Using **MinGW (g++)**:

```
make
```

This will generate:

```
bin/
├── dll_injector.exe   → GUI injector
├── dll.dll            → Payload
├── target.exe         → Test program
```

---

## ▶️ Usage

### 1. Run target program

```
.\bin\target.exe
```

### 2. Get PID

The program prints its PID directly:

```
PID: 1234
```

### 3. Inject DLL (GUI)

```
.\bin\dll_injector.exe
```

Then:

* Enter PID
* Enter full DLL path (recommended)
* Click **Inject**

---

## 🧠 How It Works

1. `OpenProcess` → get handle to target
2. `VirtualAllocEx` → allocate memory
3. `WriteProcessMemory` → write DLL path
4. `CreateRemoteThread` → call `LoadLibraryA`
5. DLL is loaded into target process

---

## ⚠️ Important Notes

* Injector and target **must have same architecture (x86/x64)**
* Always use **absolute DLL path**
* Run as **administrator** if needed

---

## 🚀 Future Improvements

* Manual mapping (no LoadLibrary)
* Process list in GUI
* File picker for DLL
* Stealth injection techniques

---

## 🧩 Skills Demonstrated

* C / C++
* Win32 API
* DirectX11
* ImGui integration
* Memory manipulation

---

## ⚠️ Disclaimer

This project is for **educational purposes only**.

Do not use on software you do not own or have permission to analyze
