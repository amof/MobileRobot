#Framing

Here is the detail of the framing process used in this application.

###Common

|Header     |Frame Number      |Command      |Number of Data(s) byte(s)    |Data          |CRC      |Footer      |
|:---------:|:----------------:|:-----------:|:---------------------------:|:------------:|:-------:|:----------:|
|1 byte     | 1 byte           | 1 byte      | 1 byte                      |Up to 80 bytes| 1 byte  | 1 byte     |

#### Header
The header is composed by a unique code : **0x10**
#### Frame Number
The frame is number with a number between 0-255.
#### Command 
All the commands are listed below: 

|       | Code  	   | Signification|
|:-----:|:------------:|:------------:|
|   1   | *0A*         | ACK          |
|   2   | ?        | NACK          |
|   3   | ?         | CCS (Control Connection Status)          |
|   4   | ?         | Target Coordinate          |
|   5   | ?         | Manual Control          |
|   6   | ?         | DIA (Data information from Acceleromter)          |
|   7   | ?         | DIU (Data information from Ultra-sonic module)          |
|   8   | ?         | DIC  (Data information from Compass)         |

####Number of Data(s) byte(s)
Number of bytes of datas in the field Data
#### Data 
Datas bytes.
#### CRC 
The CRC used is *CRC-8* and is protecting the datas.
See fellow details [here](https://users.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf).
#### Footer
The footer is composed by a unique code : **0x08**

###PC -> Mobile Robot
**Warning :** The AVR in the mobile robot will not use regular expression <regexp> library because RAM space is limited. 
List of command usable and format : 

|       | Code		   | Signification| Format       |
|:-----:|:------------:|:------------:|:------------:|
|   ?   | ?         | Target Coordinate. X is Nord(N) or Sud(S), xxxx is distance in cm ; X is East(E) or West(W), yyyy is distance in cm| X:xxxxY:yyyy| 

###Mobile Robot -> PC
Here, with the power of the pc, the regular expression will be used.

When the frame will be sended, each field will be separated by **|**.
