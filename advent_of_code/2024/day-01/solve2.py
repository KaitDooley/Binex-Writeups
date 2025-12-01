total = 0
i = 0
with open('col1.txt', 'r') as file1: # read column 1
    # parse file1 line by line
    for line1 in file1:
        line1 = int(line1.strip()) # make number
        with open('col2.txt', 'r') as file2: # read column 2
            # parse file2 line by line, count times line1 number appears in file2
            for line2 in file2:
                line2 = int(line2.strip()) # make number
                if (line1 == line2): 
                    i += 1
            total += line1 * i # increase total by line1 number times count
            i = 0 #reset i to 0
print(total)                 

