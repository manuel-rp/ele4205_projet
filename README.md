## ELE4205 Projet automne 2020 - Équipe 43

This Git repo contains the source code of the semester project in **ELE4205 - Système d'exploitation et interfaces matérielles** class, at Polytechnique Montréal.

- The project consists of a server/client application, which are connected through a TCP/IP socket. 
- A remote server hosted on Odroid-C2 captures a video stream if lighting conditions are appropriate. 
- The video stream is sent to a client application running on Ubuntu 16.04 OS, where it is displayed. 
- A button can be pressed from the server side to save pictures on the client's filesystem. 
- Saved pictures are processed using the ZBar library to detect QRCodes. 
- If a QRCode was successfuly read, the QRCode's data will be sent back to the server, which will convert it to ITU morse code and play the corresponding tune on a buzzer.

Further detail can be found on the course's Moodle website.

This Git repo was maintained by team 43:

        - Manuel Roger-Proulx    (1899033)
        - Valérie Servranckx     (1893677)
        
---

## Installation / Getting started

#### Odroid-C2 
1. Flash an Odroid-C2 with provided .sdcard file, which includes **OpenCV v3.1**.
2. Commands to initialize GPIOs: 

        $ echo 228 > /sys/class/gpio/export
        $ echo in > /sys/class/gpio/gpio228/direction

3. Commands to load PWM driver:

        $ cd /sys/devices/
        $ modprobe pwm-meson
        $ modprobe pwm-ctrl
        
4. Electrical connections: See Figure 1 of project instructions for electrical connections. Also connect buzzer to pin 33 (#108) and to ground.

#### Development environment
1. Clone repo localy on provided Ubuntu 16.04 VM, which includes **OpenCV v2.4.9**.
2. Install ZBar library using command

        $sudo apt-get install libzbar-dev libzbar0
        
3. Run **cmakeClient** and **cmakeBuzzer** bash scripts to generate out-of-source eclipse projects and executables.
4. Use the following instructions to build the server executable:
        
        $ mkdir -p build_server
        $ cd build_server
        $ source /opt/poky/2.1.3/environment-setup-aarch64-poky-linux
        $ cmake -v -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../server
        $ make

5. Secure copy cross-compiled executables **/build_server/bin/debug/ProjetServeur** and **/build_buzzer/bin/debug/Buzzer** to the same directory on Odroid using the `scp` command.
6. Launch ProjetServeur on Odroid first, then launch **/build_client/bin/debug/ProjetClient** on the local machine and follow the instructions on screen.

---