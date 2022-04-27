# Ascii Art and Image Conversion 

ascii.cpp turn a specific ascii txt file into bmp image
inverse.cpp turn a 8-bit RGB .bmp file into ascii art .txt file  


\> g++ ascii.cpp bmp.cpp -o ascii
\> g++ inverse.cpp bmp.cpp -o inverse

\> .\inverse <art.txt> <art.bmp>
\> .\ascii <input.bmp> <output.txt>

<art.txt> is path to ASCII art file.
<art.bmp> is path of output bitmap.

<input.bmp> is path to bitmap.
<output.txt> is path to output ASCII art file, if choose color ascii, type <output.html>
