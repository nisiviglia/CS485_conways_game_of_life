#	Honors Scripting Language Project

## 	Project Webmind
* Scripting language for Convoy's Game of Life
* Honors Course Enhancement Project

##	Objectives: 
* Create parser for scripting language for Project Webmind
* Call Object Library APIs to initialize worlds

##	Build Instructions (linux)

In order to build generate the build files, execute the following commands into
your command line:

Without Unit Tests

	$ cd build
	$ cmake ..
	$ make

With Unit Tests

	$ cd build
	$ cmake -DBUILD_TESTS=ON ..
	$ make


##	Run Instructions

Run Tests

	$ cd build/scripting_language
	$ ./ScriptTest

Run Custom Script

	$ cd build/scripting_language
	$ ./honors_script <scriptFile>
- Make sure your `scriptFile` is in the same director as the executable 
- or be sure to enter the correct path to your `scriptFile`


#	Grammer to implement:

*	UPPERCASE symbols are nonterminals
*	lowercase symbols are terminals		

```

SCRIPT := CREATE_ENV STMT
CREATE_ENV := CREATE_WORLD CREATE_BOARD?
CREATE_WORLD := name “WORLD_NAME” CREATE_BOARD
CREATE_BOARD := board COORD
STMT := CMD | CMD STMT
//	CMD := LIVE_CMD | DEAD_CMD | INSERT_THING | TOGGLE_CMD #can't insert_thing
CMD := LIVE_CMD | DEAD_CMD | TOGGLE_CMD
LIVE_CMD := live at ([COORD ]+ | COORD:COORD | COORD:RANGE)
DEAD_CMD := dead at ([COORD ]+ | COORD:COORD | COORD:RANGE)
TOGGLE_CMD := toggle at ([COORD ]+ | COORD:COORD | COORD:RANGE)
//	INSERT_THING := object “OBJECT_NAME” at (COORD,COORD)
COORD := ICONST,ICONST
RANGE := ICONSTxICONST
ICONST := [0-9]+
//	OBJECT_NAME := NAME
WORLD_NAME := NAME
NAME := [a-zA-Z0-9]+

```

##	Comments:

- use double forward slash to comment --> '//'
- everything to the right of '//' till newline '\n' is comment


##	Example Script:

```
name "world_cli"	// creates 
board 100,100 	// same as Board[100,100]
live at 3,3 4,4 5,6
live at 7,7:9,10
live at 1,5:4x4

// Object "glider" at 2,2 // not implemented
```

#	Tokens:

##	Keywords:

* NAME_KEY 	- name
* BOARD_KEY	- board
* AT_KEY		- at
* LIVE_KEY	- live
* DEAD_KEY	- dead
* TOGGLE_KEY	- toggle
* // OBJECT_KEY	- object

##	Other:

* COLON		- :
* COMMA		- ,
* EX			- x
* NAME		- [a-zA-Z0-9]+
* ICONST		- [0-9]+
* QUOTE		- "
