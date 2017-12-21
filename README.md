This is a PPI (Pi Power Interface) for [bakery](https://github.com/vChrisR/bakery). A PPI enables bakery to remotely power on and power off Raspberry Pis.

Configuration must be provided in ppiConfig.json. See example_ppiConfig.json for an example. Both the ppi binary and The ppiConfig.json (or symlink) must be placed in the working directory of bakery itself.

This ppi assumes a very simple web api:
* poweron: <configured url>/poweron/<id>
* poweroff: <configured url>/poweroff/<id>

<configured url> is read from the ppiConfig.json file. <id> is de an ID by which the Power manager (physical part) can idintify which port of the power manager to poweron of poweroff. The relation between PiId and <id> is stored in ppiConfig.json

Two example implementations for a power manager are in the espNode and arduinoYunPPM directories
