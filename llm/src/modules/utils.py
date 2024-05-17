import os
import yaml

class Utils(object):
    """
    Class to store util functions
    """
    def appendPath(rootPath : str, paths : list) -> str:
        """
        Static method to recurrently append paths in a long path
        """
        rootPath = os.path.join(rootPath, paths[0])
        if len(paths) == 1:
            return rootPath
        else:
            return Utils.appendPath(rootPath, paths[1:])
        
    def readYaml(filePath : str) -> dict:
        """
        Static method to read from a yaml file
        """
        content : dict
        with open(filePath, "r") as file:
            content = yaml.safe_load(file)

        return content

    def writeYaml(filePath : str, content : dict):
        """
        Static method to write in a yaml file
        """
        with open(filePath, "w") as file:
            yaml.safe_dump(content, file)