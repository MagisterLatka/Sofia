import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
os.chdir('./../')
premakeInstalled = PremakeRequirements.Validate()

if (premakeInstalled):
    if platform.system() == "Windows":
        print("\nRunning premake...")
        subprocess.call([os.path.abspath("./scripts/Win-GenProjects.bat"), "nopause"])

    print("\nSetup completed!")
else:
    print("Sofia Engine requires Premake to generate project files.")
