# Bee Space
Scripts to plot spec data in bee colour space. See http://link.springer.com/article/10.1007%2FBF00199331

* Data can be plotted in a colour hexagon or as a 3D scatter plot
* 3D scatter plot can either interactive plotly plot that can be embedded in websites
  or an animated gif of a rotating plot
  
Requirements:
------------

- [python](https://www.python.org)
- [Anaconda Scientific python package](http://continuum.io/downloads)
- [plotly python library](https://plot.ly/python/)
- [wxPython](http://www.wxpython.org/)
- [Gooey](https://github.com/chriskiehl/Gooey)
- [ImageMagick](http://www.imagemagick.org/script/index.php)

Contents
--------
- Scripts/ParseSpec.py: python script to convert spec reading to BeeSpace and create plots
- Scripts/Hexagon.R: R script to plot results in colour hexagon (used by ParseSpec.py)
- Data/BeeSensitivity.txt: Sensitivity values for each receptor in bee eyes
- Data/Background.txt: Spec values for Daylight and for background reflectance off leaves

Usage
-----
- Open command prompt and change to BeeSpace directory:
    - eg: ```cd Desktop\BeeSpace```
- Type ```python Scripts\ParseSpec.py -m [hexagon | plotly | rotate] -o outfile data_folders```

Runs beespace on all '.CSV' or '.txt' files in each folder than uses the data to draw the 
specified plot, using the folder names as sample names (can also use files with multiple
columns rather than folders with multiple files)

Graphical User Interface
------------------------
- Open command prompt Using Elevated User Rights and change to BeeSpace directory:
    - eg: ```cd Desktop\BeeSpace```
- Type ```python Scripts\BeeSpace.py``` (for mac, you need ```python.app Scripts/Beespace.py```)
- Follow instructions in window

Testing
-------
```python Scripts\ParseSpec.py Examples\Test1\*```

- Plot (scatter.png) should show same pattern as Test\scatter.png

Installing on Windows
---------------------
- Download and install the [Anaconda](http://continuum.io/downloads) package for scientific python

- Install Graphical User Interface
  - open Anaconda Command Prompt
  - type ```conda install wxpython```
  - then type ```pip install gooey```

- Making Graphical version clickable (this doesn't work because of write permissions for the output file)
  - open file browser, then click on "Organize" and "Folder and Search Options"
  - click on the "View" tab, then check "Show hidden files, folders, or drives"
  - click "OK"
  - right-click on BeeSpace.py in the Scripts folder, then click on "Open with"
  - select "Choose default program"
  - click on "Browse"
  - navigate to "C:\Users\USERNAME\AppData\Local\Continuum\Anaconda\pythonw.exe"
  - click "open", then "OK"
  - the program should now launch when double-clicked
  
- Install plotly python library:
  - open Anaconda Command Prompt
  - type ```conda install -c https://conda.binstar.org/plotly plotly```
  - follow [these instructions](https://plot.ly/python/getting-started/) to set up plotly API credentials

- Install Via GitHub:
  - Download and install the [GitHub](https://windows.github.com/) app
  - launch GitHub shell and type "git clone https://github.com/hobrien/Bee_space"
  - open Command Prompt, then type "python Desktop\GitHub\Bee_space\Test\test_ParseSpec.py"
  
- Install [ImageMagick](http://www.imagemagick.org/script/binary-releases.php) to create rotating GIF
  - This isn't working on the UoB managed PCs
  - Obnoxiously, the program needed has the same name as a system command, so an extra steps are necessary
  - Launch the command prompt and type:
  ```
    where convert > Desktop\GitHub\Bee_space\Scripts\IMconvert.cmd
  ```  
  - Open Notepad, then open the above file
  - Delete the line that doesn't mention ImageMagick
  - Put double quotes around the remaining line
  - Place an @ symbol at the fron of the line (no space)
  - place %* at the end of the line (after a space):
  ```
    @"C:Program Files\ImageMagick-6.9.1-Q16\convert.exe" %*
  ```
  - Save file
  
   
    
