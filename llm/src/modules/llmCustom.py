from onprem import LLM
from modules.fileSystem import FileSystem

class LlmCustsom(FileSystem):
    """
    Class to customize LLM from onprem
    """
    def __init__(self):
        self.__llm : LLM = LLM(use_larger=True, n_gpu_layers=35, confirm=False)
        self.__documents : str = self.getPaths["documents"]

    def ingestData(self):
        """
        Public method to ingest new data
        """
        llm.ingest(self.__documents)
