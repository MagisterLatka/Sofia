# Sofia Engine
Sofia Engine is a simple app and rendering engine. Currently it only supports Windows and partially Linux.

***

## Getting started
* For Windows, Visual Studio 2022 is recommended (other versions are not supported without changing scripts).
* For Linux, g++ compiler and make are required

<ins>**1. Downloading the repository**</ins>

Clone the repository: `git clone --recursive https://github.com/MagisterLatka/Sofia`

If the repository was cloned non-recursively, use `git submodule update --init` to clone the submodules.

<ins>**2. Configuring the dependencies:**</ins>

* Windows
1. Run the [Win-Setup.bat](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Win-Setup.bat) file found in `scripts` folder. This will download the required prerequisites for the project if they are not present yet.
2. After downloading and unzipping the files, the [Win-GenProjects.bat](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Win-GenProjects.bat) script file will get executed automatically, which will then generate a Visual Studio solution file for user's usage.<br />
If changes are made, or if you want to regenerate project files, rerun the [Win-GenProjects.bat](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Win-GenProjects.bat) script file found in `scripts` folder

* Linux
1. Open the terminal in the workspace
2. Run the [Linux-Setup.sh](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Linux-Setup.bat) file found in `scripts` folder (use `./scripts/Linux-Setup.sh` command). This will download the required prerequisites for the project if they are not present yet.
3. After downloading and unzipping the files, the [Linux-GenProjects.sh](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Linux-GenProjects.sh) script file will get executed automatically, which will then generate make files.<br />
If changes are made, or if you want to regenerate project files, rerun the [Linux-GenProjects.sh](https://github.com/MagisterLatka/Sofia/blob/master/scripts/Linux-GenProjects.sh) script file found in `scripts` folder (use `./scripts/Linux-GenProjects.sh` command).
4. To compile, run `make config=debug` or `make config=release`. The compiled project should be in the `./bin` folder.

### Main features:<br />
To be done

### Goals:
- Fast 2D rendering
- Viewer and editor applications
- Scripted interaction and behavior
- Animations
