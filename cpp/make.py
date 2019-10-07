tag = "#CLASSES = "

makeFileName = "makefile"
makeFileTemplateName = "MakefileTemplate"

with open(makeFileTemplateName) as file:
    make = file.read()

def makeClass(name):
    return "{}: {}.cpp {}.h\n\tg++ -O3 -c {}.cpp\n".format(name.lower(),name, name, name)

s = make.find(tag) + len(tag)
e = make.find("\n", s)
classes = make[s:e].split()
classes_o = " ".join([clas + ".o" for clas in classes])
classes_h = " ".join([clas.lower() + ".h" for clas in classes])
classes__ = " ".join([clas.lower() for clas in classes])
make = make.replace("CLASSES_H", classes_h)
make = make.replace("CLASSES_O", classes_o)
make = make.replace("CLASSES__", classes__)
make += "\n".join([makeClass(clas) for clas in classes])
with open(makeFileName, "w") as file:
    file.write(make)


