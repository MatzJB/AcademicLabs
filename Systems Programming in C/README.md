
# File Compression
Create a command line program that can compress and decompress a file using Huffman coding.
[https://en.wikipedia.org/wiki/Huffman_coding](url)

# HTTP server
A simple HTTP server, serving files.


Compilation (for the author)
```
Linux:		 gcc -g -std=c99 http-server.c lab2_standard.c semaphores.c requestParser.c
SUN Solaris:	 gcc -g -std=c99 http-server.c lab2_standard.c semaphores.c requestParser.c -Wall -pedantic -lsocket -lnsl

gcc -isystem -g -std=c99  http-server.c lab2_standard.c semaphores.c requestParser.c -Wall -pedantic -lsocket -lnsl
```

 Executable
 ```
./compile (for linux)
```

If this doesnt work, type "man bind" in the prompt
```
	-o http-server (for the suitable name of the executable out file)
```
 Remote testing
 ```
       http://<computername>.tekno.chalmers.se:5050/
       telnet--> o <host> <port>
```

## About the configuration files
```
<syntax>  ::= {<SEP> <rule> <NL>}
<rule>    ::= <word> <SEP>  <value>
	      |<comment>
	      |<empty_row>


<word>    ::= <port>
	      |<serverroot>
	      |<alias>

<value>   ::= <port>
	      |<dir>

<port>     ::= {<digit>}4+

<alt_path> ::= / <path> /
	       |./ <path> /  
	       |~/ <path> /

<path>    ::= {<dir> / <dir>} | <dir>

<dir>     ::= <string> { <digit> | <string> }


<comment> ::= -- <string>
<SEP>	  ::= <SP> | <TAB>


<digit>	  is a single number <- [0,9],
<string>  is an ordinary non-empty string 

<SEP>	  is either <TAB> '\t' or <SP> ' ', OBS: consecutive is now ok!
<NL>	  is new line

<empty row>: is an empty row with nothing more than new line and or
carriage return
```


## Alias file
```
<syntax>  ::= {<SEP> <rule> <NL>}
<rule>    ::= <path> <SEP> <path>| <path> <SEP> /| <comment>
```


## mimes_types.config file
```
<syntax>    ::= {<SEP> <rule> <NL>}
<rule>      ::= <suffix> <SEP> <mime-type>
<suffix>    ::= <string>
<mime-type> ::= <string> / <dir>
```

A good thing would be to actually use this BNF syntax and run it
through a lexer to first check that the config file and all the other
files has the right syntax, but I find this out of scope.

