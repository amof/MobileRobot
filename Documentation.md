#Framing

Here is the detail of the framing process used in this application.

When the frame will be sended, each field will be separated by **|** and is decoded using regular expressions.

|Header     |Frame Number      |Command      |Number of Data(s) byte(s)    |Data          |CRC      |Footer      |
|:---------:|:----------------:|:-----------:|:---------------------------:|:------------:|:-------:|:----------:|
|1 byte     | 1 byte           | 1 byte      | 1 byte                      |Up to 80 bytes| 1 byte  | 1 byte     |

#### Header
The header is composed by a unique code : **0x10**
#### Frame Number
The frame is number with a number between 0-255.
#### Command 
The command are listed below: 

|       | Hexadecimal  | Signification|
|:-----:|:------------:| :-----------:|
|   1   | *0A*         | ACK          |
#### Data 
Datas bytes.
#### CRC 
The CRC used is *CRC-8* and is protecting the datas.
See fellow details [here](https://users.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf).
#### Footer
The footer is composed by a unique code : **0x08**