import os
from pathlib import Path
import config
from modules.utils import Utils

class FileSystem(object):
    """
    Class to manage file system
    """
    def __init__(self) -> None:
        self.__rootPath : str = Path(os.path.abspath(__file__)).parents[2]
        self.__paths : dict = self.__loadPaths()
        self.__files : dict = self.__loadFiles()

    def __loadPaths(self) -> dict:
        """
        Private method to load paths from config
        """
        paths : dict = dict()

        for path in config.paths:
            longPath : str = Utils.appendPath(self.__rootPath, config.paths[path])
            paths.update({
                path : longPath,
            })
            os.makedirs(longPath, exist_ok=True)

        return paths
    
    def __loadFiles(self) -> dict:
        """
        Private method to load files from config
        """
        paths : dict = dict()

        for path in config.files:
            longPath : str = Utils.appendPath(self.__rootPath, config.files[path])
            paths.update({
                path : longPath,
            })
            if not os.path.exists(longPath):
                open(longPath, 'a').close()

        return paths
    
    def getFiles(self) -> dict:
        """
        Public method to get files
        """
        return self.__files
    
    def getPaths(self) -> dict:
        """
        Public method to get paths
        """
        return self.__paths