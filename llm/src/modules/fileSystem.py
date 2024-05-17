import os
import shutil
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

    def __deleteFolder(self, folder : str):
        """
        Private method to delete a folder
        """
        if not os.path.exists(folder):
            raise Exception("Folder " + folder + " does not exists")
        else:
            shutil.rmtree(folder)

    def _checkFileExists(self, filePath : str) -> bool:
        """
        Private method to check if file exists
        """
        return os.path.exists(filePath)

    def _deleteFile(self, fileKey : str):
        """
        Protected Method to delete a file
        """
        if fileKey not in self.__files:
            raise Exception("File " + fileKey + " does not exists")
        else:
            os.remove(self.__files[fileKey])

    def _deleteFolderContent(self, folderKey : str):
        """
        Protected Method to delete content of a folder
        """
        if folderKey not in self.__paths:
            raise Exception("Folder " + folderKey + " does not exists")
        else:
            self.__deleteFolder(self.__paths[folderKey])
            self._createFolder(self.__paths[folderKey])

    def _getFiles(self) -> dict:
        """
        Public method to get files
        """
        return self.__files
    
    def _getPaths(self) -> dict:
        """
        Public method to get paths
        """
        return self.__paths
    
    def _createFolder(self, folderName : str):
        """
        Public method to create a folder
        """
        os.makedirs(folderName, exist_ok=True)

    def _saveFile(self, fileName : str, content : list):
        """
        Method to save content in file
        """
        with open(fileName, "w") as file:
            file.writelines(content)