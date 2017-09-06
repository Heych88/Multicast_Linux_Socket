# Multicast Socket for Embedded Linux

Creates both a server and client multicast network for embedded Linux devices such as the Raspberry Pi, 
Intel Edison or BeagleBone, interfacing with either Wifi or Ethernet networks.

## Installing

Clone this repository on the embedded Linux device.

```
git clone https://github.com/Heych88/Multicast_Linux_Socket.git
```

## Run the Code

Open up a terminal window, if using the Raspberry Pi, or SSH remotely using putty, for Edison and BeagleBone.
Navigate to the cloned directories location and compile the file.

```
g++ -o multicast main.c

```

Then run the code with the following.

```
./multicast
```


## Usage

This file provides an introduction into Multicasting with Linux.

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
