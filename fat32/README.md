## Systemplanung und Projektentwicklung Projekt (Semester 3)
#### Project Requirements (original, in German)
Abzugeben ist eine eigene FAT-Implementierung basierend auf dem Projekt aus dem Unterricht.

Mindestanforderungen:

ADD File (File in die Partition schreiben; schon im Unterricht realisiert)
DEL File (File löschen)
PUT File (File von der Partition auf Dateisystem des Hostcomputers zurückschreiben)
DIR (Files anzeigen)
SHOW File (die von einer Datei belegten Blöcke ausgeben)  

Das Programm soll folgendermaßen getestet werden:
1) Viele kleine Dateien auf die Partition schreiben
2) Jede zweite Datei löschen
3) Eine sehr große Datei schreiben und sicherstellen, dass die in (2) entstanden Lücken korrekt genutzt werden 

Für Note 1 und 2:
Realisierung der Sub Directories

#### Instructions
- File streams6 is already compiled and running, or a new executable file for testing can be recompiled
- The project is meant to be run using Windows Command or Linux Terminal.
- The purpose of the project is to write and read binary data into a .bin data (which mimicks a floppy disk
with FAT32 system)

###### 1. Create a .bin file to mimick a floppy disk using FAT32 System
```
streams -partition {partitionName}.bin
```

###### 2. Reading metadata about the .bin data itself
```
streams -stats {partitionName}
```

###### 3. Add data to the floppy disk (in binary form)
to the root directory
```
streams -partition {partitionName} -add {userfile}
```

or to a certain subdirectory (my own implementation):
```
streams -partition {partitionName} -add {subDir/userfile}
```

###### 4. Show the blocks in memory that a certain file occupies
```
streams -partition {partitionName} -show {userfile}
```
subdirectory implementation:
```
streams -partition {partitionName} -show {subDir/userfile}
```

###### 5. Write a file stored inside the "floppy disc" .bin data back to the host computer
```
streams -partition test.bin -put laugh.png
```
write same file out with an option to rename it (extra requirement on day of project submission):
```
streams -partition test.bin -put laugh.png laugh123.png
```
subdirectory implementation:
```
streams -partition {partitionName} -put {subDir/filename} {newfilename -optional-}
```

###### 6. Show all files and subdirectories of the current (parent) directory
```
streams -partition test.bin -dir
```
subdirectory implementation:
```
streams -partition test.bin -dir ./subDir
```

###### 7. Create a subdirectory
Create a subdirectory under root directory
```
streams -partition test.bin -mkdir testdir
```
Create a subdirectory under a certain directory
```
streams -partition test.bin -mkdir testdir/subDir
```

###### 8. Delete a file from the directory
Create a file under root directory
```
streams -partition test.bin -del {filename}
```
Create a file under a subdirectory
```
streams -partition {partitionName} -del {subDir/fileName}
```
