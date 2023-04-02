# BYU—Hawaii CIS 301 Final Project

We needed to create a program written in C++ for our final project. We needed to implement Huffman encoding to compress text documents.

The requirements for the output needed to follow the "Zip301" format. This is a format created by the CIS 301 instructors for this class, it is as follows:

A ZIP301 file is in two parts: a plaintext header and a binary body.

The first several lines of the header are formatted thus:

[binary string] [whitespace] [character]
In the event that the [character] to encode is itself whitespace, we use the following surrogate encodings:

' ' <==> 'space'

'\n' <==> 'newline' (i.e. linefeed or LF)

'\r' <==> 'return' (i.e. carriage return or CR)

'\t' <==> 'tab'

After the binary strings, there is a single line of five asterisks:


'*****'

Following the asterisks, there is a single line containing the total number of bits, N, in the encoded document, followed by a newline character. (You need to know the number of bits because the Huffman encoding for an entire document may not necessarily be a multiple of 8. Hence, the final few bits of the final byte in the encoding may not be significant.)

This concludes the header. The body (in binary) begins on the next line. It is simply the contents of the original document, encoded using the codes listed in the file header. In the (likely) event that the encoding is not an exact multiple of 8 bits, the final byte will have some extra bits of padding at the end of it.
