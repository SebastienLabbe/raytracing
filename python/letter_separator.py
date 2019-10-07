alph = 'abcdefghijklmnopqrstuvwxyz\n'
filename = 'Letters001.txt'
seperated = 'a\n'
i = 1
with open(filename) as file:
    for line in file.readlines():
        if line.strip() == '':
            continue
        print(line[:-1],end='')
        ans = input()
        if ans == 'y':
            seperated += line
            seperated += alph[i]
            seperated += '\n'
            i += 1
        elif ans == 'm':
            seperated += alph[i]
            seperated += '\n'
            seperated += line
            i += 1
        else:
            seperated += line

with open('data/'+filename,'w') as file:
    file.write(seperated)
print(seperated)

