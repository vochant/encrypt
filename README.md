### Vochant Encrypt/Decrypt
------
**V**ochant **Enc**rpt/Decrypt is a strange way to safe your files.

BUGS:
1. Found NUL in your file? Try to decrypt it, maybe the file is broken.
2. Chars each line is different in Windows and \*nix.

USAGE:
1. Encrypt a file
 - `vect -e -i <input> -o <output>`
 - `encrypt <input>`
2. Decrypt file(s)
 - `vect -d <input>`
 - `decrypt <input>`
3. Encrypt files
 - `vect -e -i <input1> -i <input2> ... -o <output>`
 - `encrypt <input1> <input2> ...`
