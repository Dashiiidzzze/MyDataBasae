CC=g++

all: result.exe

result.exe: myDatabaseQuery.cpp select.cpp insert.cpp delete.cpp readJson.cpp supportFiles.cpp where.cpp
	$(CC) myDatabaseQuery.cpp select.cpp insert.cpp delete.cpp readJson.cpp supportFiles.cpp where.cpp -o result.exe

clean:
	rm -f result.exe

rebuild: clean all