total = 0
with open('col1.txt', 'r') as file1: # read column 1 as list 
    with open('col2.txt', 'r') as file2: # read column 2 as list
        both = zip(file1, file2) # join 2 lists as pairs
        for one, two in both: # iterate through pairs
            diff = int(one.strip()) - int(two.strip()) # find difference
            if (diff < 0): # make negatives positive
                diff = diff * -1
            total += diff # add difference to total
print(total)                  

