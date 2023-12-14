import os
import shutil
import glob

addon_folder = "./sample/addons/epic-online-services-godot"
readme_path = "./README.md"
output_zip = "epic-online-services-godot-addon"

os.system("scons target=template_debug dev_build=yes")
os.system("scons target=template_release")


# Make a temp folder and copy the addon_folder to it
temp_folder = "./temp"
if os.path.exists(temp_folder):
    shutil.rmtree(temp_folder)
os.mkdir(temp_folder)
shutil.copytree(addon_folder, temp_folder + "/addons/epic-online-services-godot")

new_addon_folder = temp_folder + "/addons/epic-online-services-godot"

# Copy the readme to the temp folder
shutil.copy(readme_path, new_addon_folder + "/README.md")

# Copy the license to the temp folder
shutil.copy(readme_path, new_addon_folder + "/LICENSE.md")

# Delete the .gitignore file in the temp folder
os.remove(new_addon_folder + "/bin/.gitignore")

# Delete ilk, pdb files in the new_addon_folder+"/bin" folder
glob_delete = [".ilk", ".pdb", ".exp", ".lib"]
for ext in glob_delete:
    for f in glob.glob(new_addon_folder + "/bin/*" + ext):
        os.remove(f)

# Zip the temp folder
shutil.make_archive(output_zip, "zip", temp_folder)

# Delete the temp folder
shutil.rmtree(temp_folder)

print("Release created successfully!")
