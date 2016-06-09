# SeaBattle
SeaBattle is a simple, network-based multi-player implementation of the game
[Battleship / SeaBattle](https://en.wikipedia.org/wiki/Battleship_(game)) (in German "[Schiffe versenken](https://de.wikipedia.org/wiki/Schiffe_versenken)") using C++ and Qt 5. There is a client (with an integrated server) and an additional dedicated server, which are both able to host multiple games.

## Building
SeaBattle depends on Qt 5 and Qt 5 Websockets and can be built by running:

```
$ qmake
$ make
```

The client is at `client/seabattle` and the dedicated server at `server/seabattle-server`.

## Screenshots
Screenshots below are in German but the game is available in English as well (depends on the system language):

| ![Game](https://user-images.githubusercontent.com/3035868/41205468-3e792a82-6cf4-11e8-9409-21ad7e422873.png) | ![Preparation](https://user-images.githubusercontent.com/3035868/41205469-3e9307f4-6cf4-11e8-9ef3-1d5fec5c658d.png) |
|-----|-----|
