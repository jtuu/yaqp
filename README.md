# Yet Another QST Parser
YAQP is a Phantasy Star Online Blue Burst quest file parser. It can dump parts of a quest file in binary and output the enemy counts of a quest in JSON format.

## Usage
`yaqp [-vjbdo] [quest.qst] ...`

* **-v**: verbose output
* **-j**: write enemy counts as .json
* **-b**: dump .bin
* **-d**: dump .dat
* **-o**: dump object code and function offset table (.o, .functions)

The output files are written into the source directory.

## Building
`make`

### Requirements
* make
* python3 (optional)

Linux. Building on Windows is untested but in theory it should work under MinGW etc.
