# Word Wrapping #

## Names and NetID's: ##

Ameel Jani- aj580
Shivank Agrawal-sa1461

## Testing Strategy ##

We took several steps in order to test our project. To test part 1 of the project, we created several text files within the directory of our program. We then compiled our program and ran the resulting executable using those files as arguments. Our first step to testing was to check whether the user arguments were valid: the first arg is a integer > 0 and second (optional) is a file or directory. We did this by entering negative numbers or non digit characters for the first argument and expected exit failure. For the second arg we tested it by feeding valid and invalid file and directory names.

In order to specifically test the functionality of Part 1 of the project, we created a wide  variety of text files. We included files that contained multiple paragraphs (paragraph breaks) and new lines - expected our output to be two new line characters for numerous newlines in the text and a space character for a singular newline character. We also included files that contained long individual words so as to test the functionality/peformance of the program's failure function - expected the text file to have wrapped sentences except for the long word which should have been on its own line. We also ran the program on files which contained multiple spaces between individual words and also on files that contained significant amounts of white spaces and new line characters before the first word and after the last word - expected only one space to be written when encountering numerous space characters. We also checked that this program behaves the same with non-conventional characters such as punctuation, numbers, special symbols. To ensure that our program was correctly determining whether a file exists or not, we fed through file names that did not exist and checked the return status for a 1 (indicating exit_failure). 

In order to test the functionality of Part 2, we created directories within the directory that our project files/program were in. We then placed the aforementioned files in that particular directory. We also made sure to create and place several other directories within the directory being tested - this was to check that our program does not go into any other directories. We also ensured that our program overwrites files starting with "wrap.". To ensure that our program could handle generic cases, we fed directories containing text files not in the same folder as the program and expected to see the program create wrapped text files. 
