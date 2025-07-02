# 🔐 VaultX - Secure Message & Password Vault

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Language](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Build](https://img.shields.io/badge/build-passing-success)



**VaultX** is a modular, offline, command-line tool built in pure C.  
VaultX is a zero-dependency, tamper-proof, fully offline password and encryption vault built from scratch in pure C — without any libraries.
It's designed to simulate real-world security constraints, expose the logic behind encryption, and prove that smart XOR with access control can still beat most hobbyist cracking attempts.


---

## ✅ Features

### 🔑 Admin Authentication
- XOR-encrypted password stored in `encrypted_password.txt`
- Requires both a valid password and a session-based XOR key (`e.g., 1-3-5-79-57-2`)
- Unauthorized access or code tampering is automatically blocked
- Removal or bypass of `authorize()` will trigger protection: system detects no `global_key` and halts

### 📄 Secret Message Encryption
- Input any message and generate a secure XOR key (ASCII 33–126 only)
- Encrypts each character with a unique, printable number
- Optionally saves encrypted message and key to `encrypted_text.txt` and `key.txt`

### 🔓 Decryption Module
- Decrypt from file or manual input
- Validates input, format, and structure
- Invalid keys result in garbage output, not decrypted text
- Anti-hacking logic ensures only session-matched key works

### 🧾 Password Manager (Working Phase)
- Add domain, username, password entries securely to `database.txt`
- Encryption uses session key from admin login — unique per session
- Viewing stored passwords also requires the same session key

### 📊 Password Strength Checker
- Checks character diversity:
  - Lowercase
  - Uppercase
  - Numbers
  - Special Characters
- Rejects passwords with unsafe characters
- Flags weak or unsafe passwords

### 🧠 Password Generator
- Generates passwords between 8 to 15 characters
- Includes all character types (A-Z, a-z, 0-9, special chars)
- Ensures strong entropy and printable format

---

## ⚔️ Security Design

### ❗ XOR ≠ Insecure — When Done Right

Even though XOR is mathematically reversible (`x ^ y = z`, so `z ^ y = x`), VaultX adds powerful protection:

- 🔁 **Session-Based Keying**  
  All encryption/decryption happens only under the admin session key. This means:
  - If you don’t have the original key used when data was saved
  - You will get **garbage** output even with full source access

- 🧠 **XOR Ambiguity**  
  `x ^ y = 'j'` has **many combinations**, but the correct one is unknowable without the original `y`.

- 🧱 **Tamper-Proof Logic**  
  - If someone removes or disables `authorize()`:
    - `global_key` will remain empty
    - The system treats it as a tampering attempt
    - All attempts to read or decrypt will be denied or return garbage

- 🧊 **No Admin = No Decryption**  
  Even if attacker logs in with altered code, without the correct key:
  - They become a *dummy admin*
  - No real credentials can be decrypted
  - The system still behaves, but doesn’t reveal real data

> ✅ **Without the correct session key, VaultX doesn’t just fail — it logically misleads.**
---
## 💼 Use Cases

- Learn practical encryption by writing and reading real XOR ciphers
- Offline password manager for personal use
- Understand tamper-proof access design without external libraries

---

## 🛠️ How to Run

```bash
gcc vaultx.c search.c -o vaultx.exe
```
```bash
./vaultx
```

---

## Default Admin Credentials
```bash
Password: kartik
Key:      1-3-5-79-57-2
```

## ❗ Disclaimer

> 🛡️ **Note:** VaultX is not trying to replace AES. It's proving that **strong logic and design can make even XOR a viable shield** — when used with session-locked keys, entropy, and tamper detection.


- ✅ **Stronger** than plain text or base64  
- 🧠 **Smarter** than fake hash-based checks  
- 🔒 **Totally offline**, tamper-detecting, and educational  

> ⚠️ If you lose your key, your data is **gone forever**. No resets. No backdoors.

---

## 👤 Author

**Kartik Bhartiya**  
🔗 [GitHub](https://github.com/kartikbhartiya)  
🧠 [LeetCode](https://leetcode.com/kartikbhartiya)

---

## 📈 Coming Soon

- 🧹 Entry deletion and secure file shredder  
- 📜 Access logs for admin operations  
- 🖥️ UI upgrade (GUI)


