# quixo
monte-carlo-tree-search for the board game [quixo](https://boardgamegeek.com/boardgame/3190/quixo)

## features
* efficient implementation of game rules
* configurable monte-carlo-tree-search bot 
* optional GUI using [SFML](https://www.sfml-dev.org/)
* multithreaded skill evaluation of different bots
## controls
* Bots::Player expects inputs of the form: \<source x> \<source y> \<destination x> \<destination y>
with the upper left position being (0,0)
* press "C" in the GUI to display estimated winrates for every outer position when used as source/destination