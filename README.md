# 🛡️ DEFENDER++

**DEFENDER++** is a C++-based cybersecurity toolkit that combines
practical security utilities with a web-based frontend. The project
provides tools for password security, encryption/decryption, port
scanning, and system monitoring through a custom C++ HTTP server and API
layer.

> **Educational & defensive use only.** Use the toolkit only on systems,
> networks, files, and credentials that you own or have explicit
> permission to test.

------------------------------------------------------------------------

## ✨ Features

### 🔐 Password Tools

-   **Random Password Generator**
    -   Generates passwords using selectable character types.
    -   Supports lowercase letters, uppercase letters, numbers, and
        symbols.
    -   Configurable password length.
-   **Password Strength & Breach Checker**
    -   Analyzes password characteristics such as:
        -   Length
        -   Uppercase characters
        -   Lowercase characters
        -   Digits
        -   Symbols
        -   Repeated characters
        -   Common-password status
-   **Password Attack Simulator**
    -   Educational simulation for understanding password-search
        complexity.
    -   Estimates combinations and theoretical attack time based on an
        input attempts-per-second value.
    -   Designed to demonstrate why longer and more unpredictable
        passwords are stronger.

### 🔒 Encryption & Decryption

-   Text encryption and decryption using a password.
-   File encryption and decryption.
-   Frontend interfaces for selecting encryption/decryption operations.
-   Backend API endpoints for text and file operations.

### 🌐 Port Scanner

-   Scans common ports on a specified target IP address.
-   Provides a simple interface for security and network-awareness
    testing.

### 🖥️ System Monitor

-   Displays system metrics through the backend API.
-   Provides an activity-log interface.
-   Includes a frontend dashboard for monitoring system information.

### 🌍 Custom C++ Web Server

DEFENDER++ includes its own lightweight HTTP server written in C++.

The server: - Serves the frontend files. - Routes `/api/...` requests to
backend functionality. - Handles JSON-based API requests. - Handles
multipart file uploads for encryption/decryption. - Returns appropriate
HTTP responses and status codes.

------------------------------------------------------------------------

## 🏗️ Project Structure

``` text
DEFENDER++/
│
├── backend/
│   ├── api/
│   │   ├── encryption.cpp
│   │   ├── decryption.cpp
│   │   ├── password_tools.cpp
│   │   ├── port_scanner.cpp
│   │   └── system_monitor.cpp
│   │
│   ├── passwordTools/
│   ├── encryption-system/
│   ├── decryption-system/
│   ├── server/
│   │   ├── http_server.cpp
│   │   ├── router.cpp
│   │   └── static_files.cpp
│   │
│   ├── system-monitor/
│   └── utils/
│
├── frontend/
│   ├── passwordTools/
│   ├── encryption-system/
│   ├── decryption-system/
│   ├── port-scanner/
│   ├── system-monitor/
│   ├── index.html
│   ├── style.css
│   └── js/
│
├── CMakeLists.txt
├── README.md
└── start.sh
```

------------------------------------------------------------------------

## ⚙️ Technologies Used

-   **C++**
-   **CMake**
-   **HTML5**
-   **CSS3**
-   **JavaScript**
-   **JSON**
-   **Linux / POSIX sockets**
-   **Font Awesome**
-   **Google Fonts**

------------------------------------------------------------------------

## 🚀 Getting Started

### 1. Clone the repository

``` bash
git clone https://github.com/krishal-sec/Defender.git
cd DEFENDER-A-Cyber-Security-Toolkit
```

### 2. Install the required build tools

On Arch Linux:

``` bash
sudo pacman -S cmake base-devel
```

On Debian/Ubuntu:

``` bash
sudo apt install cmake build-essential
```

### 3. Build and start the project

If the provided startup script is executable:

``` bash
./start.sh
```

If necessary:

``` bash
chmod +x start.sh
./start.sh
```

The server prints the local URL when it starts, for example:

``` text
DEFENDER++ running at http://localhost:<port>
```

Open that address in your browser.

### Manual CMake build

If you prefer to build manually:

``` bash
cmake -S . -B build
cmake --build build
```

Then run the generated server executable according to your CMake
configuration.

------------------------------------------------------------------------

## 🔌 API Overview

The backend exposes endpoints for the major toolkit functions.

  --------------------------------------------------------------------------
  Endpoint                               Purpose
  -------------------------------------- -----------------------------------
  `GET /api/system/metrics`              Retrieve system metrics

  `GET /api/system/activity-log`         Retrieve system activity
                                         information

  `POST /api/password/generate`          Generate a password

  `POST /api/password/check`             Analyze password strength

  `POST /api/password/attack-simulate`   Run password attack-complexity
                                         simulation

  `POST /api/scan/ports`                 Scan common ports

  `POST /api/encrypt/text`               Encrypt text

  `POST /api/decrypt/text`               Decrypt text

  `POST /api/encrypt/file`               Encrypt an uploaded file

  `POST /api/decrypt/file`               Decrypt an uploaded file
  --------------------------------------------------------------------------

------------------------------------------------------------------------

## 🔐 Security Notes

DEFENDER++ is intended for **education, development, security awareness,
and authorized testing**.

-   Do not scan networks or hosts without authorization.
-   Do not use password-analysis features against accounts you do not
    own or have permission to test.
-   Do not use the toolkit to gain unauthorized access.
-   Treat generated passwords and uploaded files as sensitive data.
-   Review and test cryptographic implementations carefully before using
    them for real-world security-critical applications.

------------------------------------------------------------------------

## 🎯 Project Goals

DEFENDER++ aims to provide a single toolkit for learning and
demonstrating fundamental cybersecurity concepts, including:

-   Password security
-   Password complexity
-   Encryption and decryption
-   Network port awareness
-   System monitoring
-   Client-server architecture
-   C++ backend development
-   Web frontend development
-   REST-style API communication

------------------------------------------------------------------------

## 🧪 Development

The project is organized into separate frontend, backend, API, server,
and utility components to make the code easier to develop and maintain.

The custom router separates:

``` text
/api/*  → Backend API handlers
other URLs → Frontend static files
```

This allows the same C++ server to provide both the web interface and
the backend functionality.

------------------------------------------------------------------------

## 📌 Current Status

DEFENDER++ is an actively developed educational cybersecurity toolkit.

Future improvements can include:

-   Improved authentication and authorization
-   More comprehensive system monitoring
-   Expanded port-scanning capabilities
-   Additional password-security analysis
-   Improved cryptographic implementations
-   Better error handling and logging
-   Automated testing
-   More detailed documentation

------------------------------------------------------------------------

## 👨‍💻 Author

**Naman Karna**
**Krishal Khatri**
**Nikhil Kr. Verma**
**Krishna Bahadur Thapa**

------------------------------------------------------------------------

## ⭐ Contributing

Contributions, suggestions, bug reports, and improvements are welcome.

Before submitting changes:

1.  Test the feature locally.
2.  Keep frontend and backend changes organized.
3.  Document new API endpoints.
4.  Avoid committing passwords, private keys, personal files, or other
    sensitive information.

------------------------------------------------------------------------

### 🛡️ DEFENDER++

**Protect • Monitor • Encrypt • Secure**
