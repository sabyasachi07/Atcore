
# Creating  Server Certificate/Key for AtCore
 - First , you have to install [openSSL] in you distro.
 - To check it is perfectly installed or not check openssl version.

## Step 1: Generate a Private Key
 - openssl genrsa -des3 -out server.key 1024

NOTE:- The number "1024" in the above command indicates the size of the  key.
       You can choose one of five sizes: 512, 758, 1024, 1536 or 2048(these numbers represent bits). 
       The larger sizes offer greater security, but this is offset by a penalty in CPU performance. 
       I recommend the best practice is of size 1024.

 
## Step 2: Generate a CSR (Certificate Signing Request)
 -  When entering the info for server, be sure that the FQDN matches the IP address/host name of the server.
 - openssl req -new -key server.key -out server.csr


## Step 3: Remove Passphrase from Key 
 - cp server.key server.key.org
 - openssl rsa -in server.key.org -out server.key

## Step 4: Generating a ((Self-Signed Certificate))(public key)
 - openssl x509 -req -days 365 -in server.csr -signkey server.key -out server.crt

## To create  available files in .pem format 
 - if you followed the above steps and want the above created files in .pem format click [here].

# Creating  Client Certificate/Key for AtCore
 - The process for creating client certificate/key is same as server with a little modification the FQDN for client could be any IP address/host name  except you given in server FQDN.


[Qt]:https://www.qt.io
[doxygen]:http://www.doxygen.n1/
[openSSL]:https://www.howtoforge.com/tutorial/how-to-install-openssl-from-source-on-linux/
[here]:https://stackoverflow.com/questions/991758/how-to-get-pem-file-from-key-and-crt-files
