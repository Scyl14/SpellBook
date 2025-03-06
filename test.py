import os

Path = input(f"""Path for the final build (Default ./Build):

>> """)
if Path == "":
    Path = "./Build"
os.path.join(Path)
print(Path)