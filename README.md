# 🔐 VaultX - Secure Message & Password Vault (In Development)

VaultX is a modular command-line tool built in C, designed for secure local encryption of secret messages and  password management(soon). It's lightweight, file-based, and built from scratch — no external libs, no gimmicks. Just pure logic, bitwise operations, and control.

Still in active development.

---

## ✅ Current Features

### 🔑 Admin Authentication
- XOR-encrypted password storage (`encrypted_password.txt`)
- Key-based access using structured keys like `91-42-76-...`
- Admin-only access to encryption features

### 📄 Secret Message Encryption
- Input your message
- Generates a strong printable ASCII key
- XOR-encrypts each character with a unique value
- Option to encrypted text and key to `encrypted_text.txt` and `key.txt`
- Decryption requires both encrypted file and exact key

### 🔓 Decryption Module
- Decrypt via file input or manual entry
- Handles invalid input, malformed keys, and wrong lengths
- Fail-safe if decryption logic breaks due to wrong inputs

---

## ⚙️ Upcoming Features (WIP)

- 🧾 **Password Manager**
  - Add/view/delete entries
  - Data stored in binary file (`vault.dat`)
  - Only decrypted in memory during active session

- 📊 **Password Strength Checker**
  - Checks password entropy and structure
  - Flags weak or guessable entries before storing

- 🛡️ **Audit Logging**
  - Logs all access attempts, encryption, deletions

- 💣 **Secure Shred**
  - Option to securely erase saved files (overwrite before delete)

---

# How to Run

```bash
gcc vaultx.c -o vaultx.exe
```
```bash
.\vaultx
```
---

# Admin Credentials

```bash
password:- kartik
key:- 1-3-5-79-57-2
```
---
## ❗ Disclaimer

VaultX uses XOR logic — strong for local file security and educational purposes, but **not equivalent to AES or industry-grade crypto**.  
That said, it’s safer than plain text, smarter than fake hashing, and fully offline.

> ⚠️ If you lose your key, the message is gone. No backdoors. No resets. This is by design.

---

## 👤 Author

**Kartik Bhartiya**  
🔗 [GitHub](https://github.com/kartikbhartiya)  
🧠 [LeetCode](https://leetcode.com/kartikbhartiya)

---

## 💬 Notes

- This is an ongoing personal project.
- VaultX is built to evolve — more features will be added over time.
- Contributions — None.
