# Hashsnail-Client
Hashsnail is extremly slow (because it uses only CPU) and barebones clone of the most popular password-cracking utility - **Hashcat**, but with one key difference - it uses distributed computing with client-server model to crack passwords. And this repository is the client side of the whole system. The application was written as a University DSA course final project, client side was written by me and server side by my colleagues.

# How it works
The main idea is to divide amount of work among clients based on their relative performance. For example, range of work for bruteforce attack can look something like **aaaa-zzzz**, it means client with this work range will be bruteforcing all the 4 letter permutations from a to z. We wrote an algorithm that divides amount of work for any number of clients in this manner. This is done with a performance benchmark that starts before any work does, then clients send benchmark results back to the server, so it can use the algorithm to determine amount of work for every client. Server sends file with hashes and information like "what hashing algorithm to use" and "what attack mode to run" to every client, after that all the clients start attacking simultaneously. All clients stop working roughly at the same time and send their results back to the server.

#### External libraries
- ASIO - to handle all the networking stuff
- spdlog - to log information to the console 

#### Implemented attack modes
 - Classic bruteforce
 - Mask based bruteforce
 - Dictionary attack

#### Implemented hashing algorithms
 - SHA-1 
 - MD5

# Screenshots

#### 1/1 client, hashes_example.hash
![This is an image](/examples/screenshots/1.png)

#### 1/3 client, hashes_example.hash
![This is an image](/examples/screenshots/4.png)

#### Server, hashes_example.hash
![This is an image](/examples/screenshots/5.png)

# How to use
 1) Download or recursive clone this repository.
 2) Extract to some location.
 3) Run **generate_vs_project.bat** and it will generate VS project or any other if you edit the file.
 4) Build the project.

# Server
[Server repository](https://github.com/HavZel/Hashsnail-Server.git)


