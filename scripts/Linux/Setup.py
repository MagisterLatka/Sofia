import os
import subprocess
import platform

from SetupPython import PythonConfiguration as PythonRequirements
PythonRequirements.Validate()

from SetupPremake import PremakeConfiguration as PremakeRequirements
os.chdir('./../')
premakeInstalled = PremakeRequirements.Validate()

if (premakeInstalled):
    if platform.system() == "Linux":
        print("\nRunning premake...")
        subprocess.call(["sh", os.path.abspath("./scripts/Linux-GenProjects.sh")])

    print("\nSetup completed!")
else:
    print("Sofia Engine requires Premake to generate project files.")
