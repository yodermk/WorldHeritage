### World Heritage Site Travel Game

The game data has a list of UNESCO world heritage sites with some basic data about each one.
At the start of the game, you are given a destination and placed in a starting location.
Your job is to navigate to your destination.

Each site should have a list of several other sites, randomly chosen, that are relatively
close to the one you are at, and another one or two that are a lot further away, maybe halfway
around the world.  At each turn, you can choose which path to take, hoping it gets you
closer to your destination.  The country is not listed on the path choices, only the site name,
so you better know your sites!  After you move, however, the country and latitude and longitude
are printed with your current location.

Oh, and if you hit a site you were at before, you LOSE!!!

Entering 'd' at any time will remind you of your destination, and entering 'g' will
give up and print the shortest path from the start to the destination.

The purpose of this program was to do some exploration of modern C++ and Boost, especially
the Boost Graph library.  A lot more data is loaded than is used in the game currently.
I'm thinking about doing some data science on it as well.

The data file whc-sites-2019.csv comes from https://www.kaggle.com/ujwalkandi/unesco-world-heritage-sites
but it has newlines within quotes in some CSV lines. The Python script fixes that and outputs
the file that the main program loads.

Have fun!

