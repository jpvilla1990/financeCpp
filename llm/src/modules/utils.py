import os

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