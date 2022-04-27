# Ascii Art and Image Conversion 

ascii.cpp turn a specific ascii txt file into bmp image
inverse.cpp turn a 8-bit RGB .bmp file into ascii art .txt file  

```
\> g++ ascii.cpp bmp.cpp -o ascii
\> g++ inverse.cpp bmp.cpp -o inverse
```
```
\> .\inverse <art.txt> <art.bmp>
\> .\ascii <input.bmp> <output.txt>
```

<art.txt> is path to ASCII art file.
<art.bmp> is path of output bitmap.

<input.bmp> is path to bitmap.
<output.txt> is path to output ASCII art file, if choose color ascii, type <output.html>

### Lena ascii art example
![image](https://user-images.githubusercontent.com/39010822/165588854-22860c1c-1531-4fce-a86c-32b8313f397e.png)
![image](https://user-images.githubusercontent.com/39010822/165588875-a7f6dd85-dea6-480b-8bb8-0043039911ae.png)
![image](https://user-images.githubusercontent.com/39010822/165588908-4fb61233-1921-47e3-8068-468782aed6ef.png)

### inverse bmp image on lena.txt(only support uncolored txt)
![image](https://user-images.githubusercontent.com/39010822/165589081-5a19219f-56f4-444a-8566-7d41a7b38230.png)
