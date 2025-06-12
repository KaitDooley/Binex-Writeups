# Strings Writeup
## Steps
```
$ ./strings
Have you ever used the 'strings' function? Check out the man pages!
```
This provides us with the hint to use `strings`, but when we run that there are a lot of strings. Remembering that most solutions have `{ }` we can pipe to `grep` to find the flag


## Solution
```
$ ./strings
Have you ever used the 'strings' function? Check out the man pages!
                                                                                                                    
$ strings strings | grep -e '{'
picoCTF{sTrIngS_sAVeS_Time_3f712a28}
```
