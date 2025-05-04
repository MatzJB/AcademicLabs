
......................................................................

		          Dokumentation LAB 2
		        Systemprogrammering i C
                       Matz Johansson B
	        	     
......................................................................


                                 ______                             
                                /     /\  
                               /     /::\
                              /     /::::\                          
                             /     /::::::\                         
                            /     /::::::::\                        
                           /     /::::::::::\                       
                          /     /:::::/\:::::\                      
                         /     /:::::/  \:::::\                     
                        /     /:::::/    \:::::\                    
                       /     /:::::/\     \:::::\                   
                      /     /:::::/  \     \:::::\                  
                     /     /:::::/    \     \:::::\                 
                    /     /:::::/      \     \:::::\                
                   /     /:::::/        \     \:::::\               
                  /     /:::::/          \     \:::::\               
                 /     /:::::/            \     \:::::\              
                /     /:::::/              \     \:::::\             
               /     /:::::/                \     \:::::\            
              /     /________________________\     \:::::\           
             /                                \     \:::::\          
            /__________________________________\     \:::::\         
            \                                         \::::/         
             \                                         \::/          
              \_________________________________________\/  








----------------------------------------------------------------------
best viewed in emacs (column width 70)
<C-u 70 C-x f>



 Compilation (for the author)
Linux:		 gcc -g -std=c99 http-server.c lab2_standard.c semaphores.c requestParser.c
SUN Solaris:	 gcc -g -std=c99 http-server.c lab2_standard.c semaphores.c requestParser.c -Wall -pedantic -lsocket -lnsl

gcc -isystem -g -std=c99  http-server.c lab2_standard.c semaphores.c requestParser.c -Wall -pedantic -lsocket -lnsl

 Executable
./compile (for linux)

If this doesnt work, type "man bind" in the prompt
	-o http-server (for the suitable name of the executable out file)

 Remote testing
       http://<computername>.tekno.chalmers.se:5050/
       telnet--> o <host> <port>

----------------------------------------------------------------------



Assumptions:

1) I concat an index.html if the abs_path has a "/" at the end, always.

2) I assume that everything in a response is not larger than the
   MAX_LINE, as the course homepage says, but I also assume that a row
   in a file may not be longer than MAX_ROW, no buffer overrun will
   occur however, but a line will be truncates if longer.


Disclaimer
----------

Hopefully, the code itself will be sufficient for understanding what
Im trying to do, I will however go through the main functions in this
lab, just in case.

The lab is divided into 3 parts, http-server.c, requestParser.c and
semaphores.c.

The program is located in http-server.c, where the server-loop is
located, among flag-handling and functions handling configurations.

I have created sstrcat and sstrcpy which is simple functions that
checks that no buffer overruns will take place.


The code
--------

 requestParser........................................................
This is the parser and sender of a file.
The status_line and entity_header functions is the building blocks for
the response.  this is the syntax for the first row of the request:
GET <abs_path> <http-version>

<abs_path>::=/ <path>
<http-version>::=HTTP/1.<digit>

See below (syntax for files) for definition of <path>

The status_line will take care of keep-alive and alias, and it will
always send a file to the browser.

If everything went fine, then the correct reponse and the file will be sent.

Upon error it will send an error-file, which is located in directory
./errors/ The files are HTML and is assumed there (as a part of the
program) as well as the index-file.


Semaphores............................................................ 
Is a "MPD version" of the books "Dining philosophers
solution".


lab2_standard.........................................................
In this file there is the definition of sstrcat, sstrcpy, xalloc and disp

The disp function was used in lab1 and made it lot easier to scontrol
if any output or a part of output should be printed.

The disp function was designed to work in 4 stages, the enumerator
detail, reflects the quantity of outputs you get.

The macro DETAIL is used almost everywhere as an argument to disp,
has been renamed to STYLE.

STYLE:
	nothing	= prints nothing with disp
	brief	= prints only errors (extreme>)
	gedell	= prints (did_you_know>) in a special way
	carlsson	= prints (did_you_know>) 
	stromdahl	= prints (did_you_know>) somtimes annoying 
	verbose	= prints (did_you_know>) 
	all	= prints everything

The best style for debugging is "brief".
The best for ordinary run, is gedell or carlsson.


 http-server..........................................................
Main takes care of the flags and config-file calling
parse_config_file and then start_server which takes care of the
requests.

start_server opens a port och listens for a connection.
The parent handles the server (listen and fork)

The children handles the clients (read requests and send files, and
write to logg-file). The logg-file is appended each time you start the
server among with viable information such as date, IP, Pid and the
request itself.

To be able to write to a log-file while concurrently handling new
connections, we need to lock the file.  I have created a file called
semaphore.c which contains the locking mechanism (grade 4). If a file
is corrupt of not found, the parser will send a file from errors/
directory.  This directory contains all the errors that can occur and
is required for this lab.

A nice errormessage is made by our beloved Obi-Wan Kenobi Alec
Guinness/Ewan McGregor.

 requestParser........................................................ 
Takes care of the requests and changes keep-alive and filenames
dynamically.  keep alive is a little hard to test properly, I have
however tested that I can send repeated files with telnet.

 siff_alias...........................................................
This function is similar to parse_config, it is actually from the same
code, with small but important adjustments, see syntax for more info.

 status_line..........................................................
This function check the syntax of the request that the readLine saved.
The status_line created a status_line-line, with the different
responses that was required.

 entity_headers.......................................................
Builds the content-type and content-length and keep-alive (from RFC
1945)

 send_file............................................................
Buffers a file and sends it in small portions to a client, this is
made to prevent exceptions due to buffering too large files at one go.

The function doesnt handle large files good, but I added a delay
function that pauses between sends, this allows the browser to recover
from the sends and probablility for a successful send is increased.


 Haskell functions....................................................
A small set of haskell-inspired functions (Yes, I have turned to the
light side of the force) 
invoked functions such as: tail, last, head,...

 find_appropriate_mimes...............................................
Searches in a file for the appropriate mime, if none was found then it
returns application/octet-stream, the function uses extract suffix and
matches the suffix with each row from a mime_types file.

 extract_suffix.......................................................
Used by find_appropriate_mimes to determine a mime-type to be used.
This function has been quite altered from the last submission, I have
made it a better function that handles more cases than the last one,
it now also handles a rather unusual case (".*") but I want it to be
correct.

 is_repeating(string,c)...............................................
Returns 1 if the file is illegal in the sense that is contains at
least two consecutive c.  used to see if a filename is illegal (2
conseq chars) and in strNtok3, as a special case.

 parse_config_file:(grade 4, grade5)..................................
Extracts a filename for aliases, serverroot and port.  The aliases
themselves is handled by siff_aliases which is called from here.
The function doesnt really care what it reads, it doesnt follow the syntax below.


 xalloc (and macros for memory handling, see lab2_standard.h).........
This little function is one of the few that calls disp to say that the
malloc or alloc was errornous.

About the configuration files

The syntax of config file.............................................

config file:

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


the syntax of alias file..............................................

<syntax>  ::= {<SEP> <rule> <NL>}
<rule>    ::= <path> <SEP> <path>| <path> <SEP> /| <comment>


the syntax of mimes_types.config file.................................

<syntax>    ::= {<SEP> <rule> <NL>}
<rule>      ::= <suffix> <SEP> <mime-type>
<suffix>    ::= <string>
<mime-type> ::= <string> / <dir>


A good thing would be to actually use this BNF syntax and run it
through a lexer to first check that the config file and all the other
files has the right syntax, but I find this out of scope.

Future implementations................................................

 linked_ring..........................................................
For storing the "rules" that is to be saved by readLines, it will be a
ring that is linked together where you can go right or left, with data
for element (char *) previous lengths of data (the left ones) and the
number of elements in the ring.  The ring is not supposed to support
deletage of individual elements... I'm looking forward to implement
that datastructure, but it might take awhile...

--the end