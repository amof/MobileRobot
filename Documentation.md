#Framing

Here is the detail of the framing process used in this application.

**Warning :** The AVR in the mobile robot will not use regular expression <regexp> library because RAM space is limited.

**Warning 2 :** The command code must not be used in the data field.

|Header     |Frame Number	|Number of Data(s) byte(s)	|Command      |Data          |CRC      |Footer      |
|:---------:|:-------------:|:-------------------------:|:-----------:|:------------:|:-------:|:----------:|
|1 byte     | 3 bytes		| 2 bytes					| 1 byte      |Up to 99 bytes| 1 byte  | 1 byte     |

####1. Header
The header is composed by a unique code : **0x10**
####2. Frame Number
The frame is number with a number between 001-999.
####3. Number of Data(s) byte(s)
Number of bytes of datas in the field Data : from 00 to 99 (bytes)
####4. Command & Data
All the commands and the data format are listed below: 

|       | Command  	| Data				| Signification			|
|:-----:|:---------:|:-----------------:|:---------------------:|
| General |
|   1   | *0A*      | /					| ACK|
|   2   | ?        	| /					| NACK|
|   3   | ?         | /					| CCS (Control Connection Status)|
| Manual Mode |
|   4   | ?         | /					| MM : Manual Mode|
|   5   | ?         | XY				| Control Robot(CR): XY is *UP*, *DO*wn, *LE*ft, *RI*ght|
| Automatic Mode |
|   6   | ?         | /					| Automatic Mode(AM)|
|   7   | ?         | XxxxxYyyyy		| Target Coordinate(TC): X is Nord(N) or Sud(S), xxxx is distance in cm ; Y is East(E) or West(W), yyyy is distance in cm|
| Information |
|   8   | ?         |					| DIA (Data information from Acceleromter)|
|   9   | ?         |					| DIU (Data information from Ultra-sonic module)|
|   10  | ?         |					| DIC (Data information from Compass)|

####5. CRC 
The CRC used is *CRC-8* and is protecting the datas.
See fellow details [here](https://users.ece.cmu.edu/~koopman/roses/dsn04/koopman04_crc_poly_embedded.pdf).
####6. Footer
The footer is composed by a unique code : **0x08**

