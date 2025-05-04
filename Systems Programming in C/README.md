
# File Compression
A command line program that can compress and decompress a file using Huffman coding.
[https://en.wikipedia.org/wiki/Huffman_coding](url)

```
./.a.out [-d] infile outfile
```

without the flag: plain compression
with the -d: decompression


--compilation-- (for me)
```
gcc -std=c99 MatZip.c bitsequences.c lab1_standard.c trees.c huffman.c
frequence.c -Wall -pedantic
```

## Intro
It all starts with the ratefunction. I pass an extra argument, why I
do this becomes clear later on.

The algorithm is pretty straightforward, I take a look at the pointer
to a FILE to see if the file exist. I use a new function called
disp, more about this function later. TABLE_SIZE is set to 256
meaning a file can only have a maximum of 256 different characters.
The function just loops through the file and counts the chars it passes
and increments the counter variable. I put an assert to check if the
character (we can call it ch) fetched from the file is between zero
and 255. An array 'dist' is incremented by one at element ch. When
done, the array contains the distribution rates of the characters
represented by a number from 0 to 255.
 

### Maketree

Maketree builds a binary tree from the array of trees with only one
node (2 NULL children). The array is called "lost_woods".
The function 'graft' is used to build the huffman tree, while doing
this -that is- taking two trees with the lowest frequencies together,
the third argument in graft is not important since it is just a node.

Functions always look before a NULL in the resulting tree
(see also buildTable() in **"huffman.c"**)
As for the frequency, we need to add the frequencies together since
the tree that is created the next while loop is then to be sorted
again and anew the two trees with the lowest frequencies are
'grafted'. (sorry could not come up with a better name than that)
The idea is to assign all the characters we found in ratetable with
their own uniqe binary code.

In this example we only have three elements, my goal was to limit the
time complexity, but due to lack of time I just let all elements in
lost_Woods be sorted and grafted although it is very bad.

example of makeTree
three trees with frequencies listed under the nodes respectively

```
    [0     1     2]

     a     b     e                                                
    /2\   /1\   /2\                                               
```
we sort with qsort (reversed)...                                      
```
     e     a     b                                                
    /2\   /2\   /1\           
```
graft... array is now 2 elements in size                            
```
    [0         1]
                                                                    
     e         @ 
    /2\       /3\                                                    
             a   b                                                
            /2\ /1\                                                  
```
sort... graft...
```
     e          @ 
    /2\       / 3 \
             a     b                                                
            /2\   /1\ 
```

the array is now 1 element in size and 
```
                 [0]

                  @                                             
                / 5 \                                                 
               @     e
              /3\   /2\
             a   b                                                
            /2\ /1\
```



## maketable

I use the bitshift for creating the huffman table, by shifting the
bitsequence one bit to the left.  And then + 1 (or | 1) since there
are always a zero in the sequences rightmost bit.  We must keep track
of the depth as well because the structure contains of both the mask
and how many bits the mask consist of.  The level is another word for
the number of bits, as we go through our tree we always add one bit
when we pass a node.  At the same level we have the same amount of
bits.  So now we have stored the routes to take to find a character, 0
means left and 1 means right.

### putTree & getTree
I realised that while printing the trees (as done
if DETAIL set to all) I had a function to display the trees in an
preorder traversal, the trees bacame mirrorised, weird...  getTree was
obviously errornous. Since the architectures has different orders of
evaluation we need to put Tree * left = getTree(file) and the same
for the right tree when we do the recursive call.


### encode/compression

First we must use the function that calculates the distribution of the
characters in the textfile.
We use makeTree to create the huffman tree from the array of distribution.

We now have a tree, so we create a huffmanTable to convert from a char to
a unique binary code (bitsequence).
The following order is used: filesize + tree + compressed_file


So, we store a 32 bit sequence in the beginning of the soon-to-be
compressed file. We must do this because when we store the file
bitwise, the grid in which we put the sequences is in bytes, that is
following example fully illustrates the 'problem':
 
```
0011, 00111
```
the sequences will be stored as
```
[0 ,0 ,1 ,1 ,0 ,0 ,1 ,1 ][1 , , , , , , , ]
```
the blanks are filled with zeroes since the file must have whole bytes
in it.  To know when the bit sequences stop, we must store the size of
the original file.


## decode/decompression

The first step is to build a tree, this is done by reading from the
file which has been created from when we compressed the file.
When we have done that we pass the tree to the decode function.  

The function then reads bits from after the first 32 bits (since they
have already been read and stored as a variable). In this function I
introduce a little 'ant' variable.

The ant as in 'reality' walks up the binary tree and walks on the
braches...  When we read a bit, the ant walks the tree in that
direction, left or right.  If the ant reaches a leaf, the real
character in that leaf is put in the bitfile.


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

## Configuration files
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

